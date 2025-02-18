open Cil
open Pretty
module E = Errormsg

(* Function to check if an expression contains a specific variable *)
let rec uses_variable (vname : string) (e : exp) : bool =
  match e with
  | Lval (Var vi, _) -> vi.vname = vname
  | BinOp (_, e1, e2, _) -> uses_variable vname e1 || uses_variable vname e2
  | UnOp (_, e, _) -> uses_variable vname e
  | AddrOf (Var vi, _) -> vi.vname = vname
  | CastE (_, e) -> uses_variable vname e
  | _ -> false

(* Create the function call simulate_seu_main(&var) dynamically *)
let create_seu_call (vi : varinfo) (loc : location) : instr =
  let seu_fun = findOrCreateFunc dummyFile "simulate_seu_main"
    (TFun(voidType, Some [("arg", TPtr(intType, []), [])], false, [])) in
  Call (None, Lval (Var seu_fun, NoOffset), [AddrOf (Var vi, NoOffset)], loc)

(* Visitor class to insert simulate_seu_main before variable usage *)
class seuInstrumentationVisitor (target_var : string) = object
  inherit nopCilVisitor
  val mutable inserted = false  (* Ensure only one insertion before first use *)

  method vstmt (s : stmt) =
    match s.skind with
    | Instr il -> 
        let new_instrs = ref [] in
        let added_seu_call = ref false in (* Avoid duplicate insertions *)

        List.iter (fun i ->
          match i with
          (* Array Index Case *)
          | Set ((Var vi, NoOffset), Lval (Var arr, Index (Lval (Var v_idx, NoOffset), NoOffset)), loc)
            when v_idx.vname = target_var -> 
              if not !added_seu_call then (
                let seu_instr = create_seu_call v_idx loc in
                new_instrs := seu_instr :: !new_instrs;
                added_seu_call := true
              );
              new_instrs := i :: !new_instrs

          (* Compound Assignment Case *)
          | Set ((Var vi, NoOffset), BinOp (op, Lval (Var v, NoOffset), e, typ), loc)
            when v.vname = target_var -> 
              if not !added_seu_call then (
                let seu_instr = create_seu_call v loc in
                new_instrs := seu_instr :: !new_instrs;
                added_seu_call := true
              );
              new_instrs := i :: !new_instrs

          (* Expression Usage Case *)
          | Set ((Var vi, NoOffset), e, loc)
            when uses_variable target_var e -> 
              if not !added_seu_call then (
                let seu_instr = create_seu_call vi loc in
                new_instrs := seu_instr :: !new_instrs;
                added_seu_call := true
              );
              new_instrs := i :: !new_instrs

          (* Function Argument Case *)
          | Call (ret, func, args, loc) ->
              let found_target = ref false in
              let new_args = List.map (fun arg ->
                match arg with
                | Lval (Var vi, NoOffset) when vi.vname = target_var -> 
                    found_target := true; arg
                | _ -> arg
              ) args in

              if !found_target && not !added_seu_call then (
                let seu_instr = create_seu_call (makeGlobalVar target_var intType) loc in
                new_instrs := seu_instr :: !new_instrs;
                added_seu_call := true
              );

              let new_call = Call (ret, func, new_args, loc) in
              new_instrs := new_call :: !new_instrs

          | _ -> new_instrs := i :: !new_instrs
        ) il;

        ChangeTo (mkStmt (Instr (List.rev !new_instrs)))

    (* If Condition Case *)
    | If (cond, b1, b2, loc) when uses_variable target_var cond -> 
        let seu_instr = create_seu_call (makeGlobalVar target_var intType) loc in
        ChangeTo (mkStmt (Block (mkBlock [mkStmtOneInstr seu_instr; s])))

    (* While Loop Case *)
    | Loop (b, loc, _, _) when uses_variable target_var (Lval (Var (makeGlobalVar target_var intType), NoOffset)) -> 
        let seu_instr = create_seu_call (makeGlobalVar target_var intType) loc in
        let new_loop_stmt = Loop (b, loc, None, None) in
        ChangeTo (mkStmt (Block (mkBlock [mkStmtOneInstr seu_instr; mkStmt new_loop_stmt])))

    (* Logical Expression Case *)
    | If (BinOp ((LAnd | LOr), Lval (Var vi1, NoOffset), Lval (Var vi2, NoOffset), _), b1, b2, loc)
      when vi1.vname = target_var || vi2.vname = target_var -> 
        let seu_instr1 = create_seu_call vi1 loc in
        let seu_instr2 = create_seu_call vi2 loc in
        let new_block = mkBlock [mkStmtOneInstr seu_instr1; mkStmtOneInstr seu_instr2; s] in
        ChangeTo (mkStmt (Block new_block))

    (* Pointer Dereferencing Case - Only before dereferencing *)
    | Instr [Set ((Var vi, NoOffset), Lval (Mem e, NoOffset), loc)]
      when uses_variable target_var e -> 
        let seu_instr = mkStmtOneInstr (create_seu_call vi loc) in
        ChangeTo (mkStmt (Block (mkBlock [seu_instr; s])))

    (* Switch Statement Case *)
    | Switch (exp, body, cases, loc) when uses_variable target_var exp -> 
        let seu_instr = mkStmtOneInstr (create_seu_call (makeGlobalVar target_var intType) loc) in
        ChangeTo (mkStmt (Block (mkBlock [seu_instr; s])))

    | _ -> DoChildren
end

(* Process function to insert instrumentation *)
let process_function (fd : fundec) (target_var : string) : unit =
  let has_target = List.exists (fun vi -> vi.vname = target_var) (fd.sformals @ fd.slocals) in
  if has_target then ignore (visitCilFunction (new seuInstrumentationVisitor target_var) fd)

(* Prevents CIL from expanding arrays into explicit loops *)
let prevent_array_expansion (f : file) : unit =
  iterGlobals f (function
    | GVar (vi, { init = Some (CompoundInit (TArray (_, _, _), _)) }, loc) -> 
        vi.vtype <- TArray (vi.vtype, None, [])
    | _ -> ())

(* Entry point: Parses the C file and modifies it *)
let () =
  if Array.length Sys.argv <> 4 then (
    Printf.eprintf "Usage: %s <source_file> <output_file> <variable>\n" Sys.argv.(0);
    exit 1
  );

  let input_file = Sys.argv.(1) in
  let output_file = Sys.argv.(2) in
  let target_var = Sys.argv.(3) in

  let f = Frontc.parse input_file () in
  prevent_array_expansion f;
  iterGlobals f (function
    | GFun (fd, _) -> process_function fd target_var
    | _ -> ());

  let out_channel = open_out output_file in
  Cil.dumpFile defaultCilPrinter out_channel output_file f;
  close_out out_channel
