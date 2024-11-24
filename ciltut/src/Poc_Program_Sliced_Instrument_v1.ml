open Cil
open Pretty

module L = List

(* This class will traverse the AST and instrument uses of the variable "x" *)
class variableInstrumenterClass (target_var_name : string) = object(self)
  (* Inherit from the default nop visitor, which does nothing *)
  inherit nopCilVisitor

  (* This method is called on statements. We'll override it to handle variable usage. *)
  method vstmt (s : stmt) =
    let action s =
      match s.skind with
      (* Look for instructions within the statement *)
      | Instr il ->
          (* Instrument each instruction if it uses the target variable *)
          let new_instrs = L.fold_left (fun acc i ->
            match i with
            | Set((Var vi, NoOffset), _, loc) when vi.vname = target_var_name ->
                (* If the assignment targets "x", just keep it *)
                i :: acc
            | Call(_, Lval(Var vi, NoOffset), _, loc)
            | Set(_, Lval(Var vi, NoOffset), loc)
            | Call(_, _, Lval(Var vi, NoOffset) :: _, loc) when vi.vname = target_var_name ->
                (* If the instruction involves "x", add an assignment of "x = 7" before it *)
                let assign_instr = Set(var vi, integer 7, loc) in
                assign_instr :: i :: acc
            | _ -> i :: acc
          ) [] il in
          (* Replace the instructions in reverse order to maintain the original order *)
          s.skind <- Instr (List.rev new_instrs);
          s
      | _ -> s
    in
    (* Visit children and apply the action *)
    ChangeDoChildrenPost(s, action)
end

(* Instrument a single function to insert assignments before uses of the variable "x" *)
let processFunction (fd : fundec) (loc : location) (target_var_name : string) : unit =
  let vis = new variableInstrumenterClass target_var_name in
  ignore(visitCilFunction vis fd)

(* Iterate over all functions in the file and apply the instrumentation *)
let instrumentVariable (f : file) (target_var_name : string) : unit =
  iterGlobals f (function
    | GFun (fd, loc) -> processFunction fd loc target_var_name
    | _ -> ()
  )
  
let () = 
  (* Parse the C file you want to instrument *)
  let f = Frontc.parse "../test/Poc_Program_Sliced.c" () in
  
  (* Name of the variable to instrument, e.g., "vname" *)
  let vname = "x" in
  instrumentVariable f vname;

  (* Output the modified AST to a new C file *)
  let out_channel = open_out "../test/Poc_Program_Sliced_Instrument_modified.c" in
  Cil.dumpFile defaultCilPrinter out_channel "../test/Poc_Program_Sliced_Instrument_modified.c" f;
  close_out out_channel