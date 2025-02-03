open Cil
open Pretty
module E = Errormsg

(* Function to check if an expression contains the target variable *)
let rec has_variable (vname : string) (e : exp) : bool =
  match e with
  | Lval (Var vi, NoOffset) -> vi.vname = vname
  | BinOp (_, e1, e2, _) -> has_variable vname e1 || has_variable vname e2
  | UnOp (_, e1, _) -> has_variable vname e1
  | CastE (_, e1) -> has_variable vname e1
  | _ -> false

(* Function to create simulate_seu_main(&x) call *)
let create_seu_call (vi : varinfo) (loc : location) : instr =
  let seu_fun = makeGlobalVar "simulate_seu_main" (TFun(voidType, Some [("x", TPtr(intType, []), [])], false, [])) in
  Call (None, Lval (Var seu_fun, NoOffset), [AddrOf (Var vi, NoOffset)], loc)

(* Visitor class to insert simulate_seu_main before each use of x *)
class seuInstrumentationVisitor (target_var : string) = object
  inherit nopCilVisitor

  method vstmt (s : stmt) =
    match s.skind with
    | Instr il -> 
        let new_instrs = ref [] in
        List.iter (fun i ->
          match i with
          | Set ((Var vi, NoOffset), rhs, loc) when has_variable target_var rhs ->
              new_instrs := create_seu_call vi loc :: i :: !new_instrs
          | Call (_, Lval (Var vi, NoOffset), args, loc) 
            when List.exists (has_variable target_var) args ->
              new_instrs := create_seu_call vi loc :: i :: !new_instrs
          | _ -> new_instrs := i :: !new_instrs
        ) il;
        ChangeTo (mkStmt (Instr (List.rev !new_instrs)))
    | If (cond, b1, b2, loc) when has_variable target_var cond ->
        let new_stmt = mkStmt (Instr [create_seu_call (makeLocalVar (emptyFunction "temp") target_var intType) loc]) in
        let new_block = mkBlock (new_stmt :: [s]) in
        ChangeTo (mkStmt (Block new_block))
    | _ -> DoChildren
end

(* Process each function, ensuring `simulate_seu_main(&x);` is added *)
let process_function (fd : fundec) (target_var : string) : unit =
  let has_target = List.exists (fun vi -> vi.vname = target_var) (fd.sformals @ fd.slocals) in
  if has_target then ignore (visitCilFunction (new seuInstrumentationVisitor target_var) fd)

(* Entry point: Parses the C file and modifies it *)
let () =
  let input_file = "../test/input.c" in
  let output_file = "../test/output.c" in
  let target_var = "x" in  (* Variable to monitor *)

  let f = Frontc.parse input_file () in
  iterGlobals f (function
    | GFun (fd, _) -> process_function fd target_var
    | _ -> ());

  let out_channel = open_out output_file in
  Cil.dumpFile defaultCilPrinter out_channel output_file f;
  close_out out_channel
