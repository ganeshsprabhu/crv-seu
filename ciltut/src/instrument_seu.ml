open Cil
open Pretty
module E = Errormsg

(* Function to check if an expression contains a specific variable *)
let rec uses_variable (vname : string) (e : exp) : bool =
  match e with
  | Lval (Var vi, NoOffset) -> vi.vname = vname
  | Lval (host, offset) ->
      (match offset with
       | Field (fi, _) when fi.fname = vname -> true
       | Index (idx, _) -> uses_variable vname idx
       | _ -> false)
  | BinOp (_, e1, e2, _) -> uses_variable vname e1 || uses_variable vname e2
  | UnOp (_, e, _) -> uses_variable vname e
  | AddrOf (Var vi, _) -> vi.vname = vname
  | CastE (_, e) -> uses_variable vname e
  | _ -> false

(* Extract the first matching lval from expression where variable is used *)
let rec extract_matching_lval (vname : string) (e : exp) : lval option =
  match e with
  | Lval lv ->
      (match lv with
       | (Var vi, NoOffset) when vi.vname = vname -> Some lv
       | (Var vi, Field (fi, _)) when fi.fname = vname -> Some lv
       | (Mem (Lval (Var vi, _)), Field (fi, _)) when fi.fname = vname -> Some lv
       | (Var vi, Index (idx, _)) when uses_variable vname idx -> Some (Var vi, Index (idx, NoOffset))
       | _ -> None)
  | BinOp (_, e1, e2, _) ->
      (match extract_matching_lval vname e1 with
       | Some lv -> Some lv
       | None -> extract_matching_lval vname e2)
  | UnOp (_, e1, _) -> extract_matching_lval vname e1
  | CastE (_, e1) -> extract_matching_lval vname e1
  | _ -> None

(* Create the function call simulate_seu_main(&x) dynamically *)
let create_seu_call (lv : lval) (loc : location) : instr =
  let seu_fun = findOrCreateFunc dummyFile "simulate_seu_main"
    (TFun(voidType, Some ["arg", TPtr(intType, []), []], false, [])) in
  Call (None, Lval (Var seu_fun, NoOffset), [AddrOf lv], loc)

class seuInstrumentationVisitor (target_var : string) = object
  inherit nopCilVisitor

  method vstmt (s : stmt) =
    match s.skind with
    | Instr il ->
        let new_instrs = ref [] in
        let inserted_here = ref false in
        List.iter (fun i ->
          match i with
          | Set (_, rhs, loc) ->
              new_instrs := !new_instrs @ [i];
              if uses_variable target_var rhs && not !inserted_here then (
                match extract_matching_lval target_var rhs with
                | Some matched_lv ->
                    new_instrs := !new_instrs @ [create_seu_call matched_lv loc];
                    inserted_here := true
                | None -> ()
              )

          | Call (_, _, args, loc) ->
              new_instrs := !new_instrs @ [i];
              if List.exists (uses_variable target_var) args && not !inserted_here then (
                try
                  let arg = List.find (uses_variable target_var) args in
                  match extract_matching_lval target_var arg with
                  | Some matched_lv ->
                      new_instrs := !new_instrs @ [create_seu_call matched_lv loc];
                      inserted_here := true
                  | None -> ()
                with Not_found -> ()
              )

          | _ -> new_instrs := !new_instrs @ [i]
        ) il;
        ChangeTo (mkStmt (Instr !new_instrs))

    | If (cond, b1, b2, loc) when uses_variable target_var cond ->
        let lv = Var (makeGlobalVar target_var intType), NoOffset in
        let call = mkStmtOneInstr (create_seu_call lv loc) in
        ChangeTo (mkStmt (Block (mkBlock [call; s])))

    | Loop (body, loc, _, _) ->
        let lv = Var (makeGlobalVar target_var intType), NoOffset in
        let call = mkStmtOneInstr (create_seu_call lv loc) in
        ChangeTo (mkStmt (Block (mkBlock [call; s])))

    | Switch (exp, body, cases, loc) when uses_variable target_var exp ->
        let lv = Var (makeGlobalVar target_var intType), NoOffset in
        let call = mkStmtOneInstr (create_seu_call lv loc) in
        ChangeTo (mkStmt (Block (mkBlock [call; s])))

    | _ -> DoChildren
end

let () =
  if Array.length Sys.argv <> 4 then (
    Printf.eprintf "Usage: %s <input_file> <output_file> <variable>\n" Sys.argv.(0);
    exit 1
  );
  let input_file = Sys.argv.(1) in
  let output_file = Sys.argv.(2) in
  let target_var = Sys.argv.(3) in
  let f = Frontc.parse input_file () in
  iterGlobals f (function
    | GFun (fd, _) -> ignore (visitCilFunction (new seuInstrumentationVisitor target_var) fd)
    | _ -> ());
  let out_channel = open_out output_file in
  dumpFile defaultCilPrinter out_channel output_file f;
  close_out out_channel
