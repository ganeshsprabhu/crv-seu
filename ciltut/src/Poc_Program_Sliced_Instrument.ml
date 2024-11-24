open Cil
open Pretty

module L = List
module E = Errormsg

(* This class will traverse the AST and instrument all uses of the variable "x" *)
class variableInstrumenterClass (target_var_name : string) = object(self)
  (* Inherit from the default nop visitor, which does nothing *)
  inherit nopCilVisitor

  (* Helper method to check if an expression involves the target variable *)
  method private is_target_variable (e : exp) =
    let rec aux e = 
      match e with
      | Lval(Var vi, NoOffset) when vi.vname = target_var_name -> true
      | BinOp(_, e1, e2, _) -> aux e1 || aux e2
      | UnOp(_, e, _) -> aux e
      (* | CastE(_, e) -> aux e *)
      (* | Lval(Mem e, _) -> aux e *)
      (* | AddrOf lv | StartOf lv -> self#is_target_lv lv *)
      | _ -> false

    (* and self#is_target_lv lv =
      match lv with
      | Var vi, NoOffset when vi.vname = target_var_name -> true
      | Mem e, _ -> aux e
      | _, Field(_, _) | _, Index(_, _) -> false
      | _ -> false *)
    in
    aux e

  (* Override to handle expressions involving the target variable *)
  method vexpr (e : exp) =
    match e with
    | Lval(Var vi, NoOffset) when vi.vname = target_var_name ->
        (* Skip further exploration as we will handle instrumentation at statement level *)
        SkipChildren
    | _ -> DoChildren

  (* Override to handle statements containing the target variable *)
  method vstmt (s : stmt) =
    let action s =
      let create_assign_stmt vi loc =
        (* Create an assignment "x = 7" *)
        mkStmt (Instr [Set(var vi, integer 7, loc)])
      in

      match s.skind with
      | If(cond, b1, b2, loc) when self#is_target_variable cond ->
          (* If the condition of the if statement involves the target variable *)
          let vi = makeVarinfo false target_var_name intType in
          let assign_stmt = create_assign_stmt vi loc in
          (* Create a new block that includes the assignment before the original if-statement *)
          let new_block = mkBlock [assign_stmt; s] in
          s.skind <- Block new_block;
          s

      | Instr il ->
          (* Handle instructions, checking for the target variable *)
          let new_instrs = L.fold_left (fun acc i ->
            match i with
            | Set((Var vi, NoOffset), e, loc) when self#is_target_variable e ->
                (* Add assignment before the variable is used *)
                let assign_instr = Set(var vi, integer 7, loc) in
                assign_instr :: i :: acc
            | Call(_, Lval(Var vi, NoOffset), _, loc)
            | Call(_, _, Lval(Var vi, NoOffset) :: _, loc)
            | Set(_, Lval(Var vi, NoOffset), loc) when vi.vname = target_var_name ->
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