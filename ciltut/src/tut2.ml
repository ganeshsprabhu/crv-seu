open Cil
open Pretty
open Myownciltut
module E = Errormsg

(* open Tututil  *)

(* methods exposed by nopCilVisitor *)
(* 
  SkipChildren: stops the visitor from recursing into child AST nodes
  DoChildren: recurses into child AST nodes
  ChangeTo x: replace the AST node with new node "x"
*)

(* visit a function, and remove all assignments to a global variable called "deleted" *)
class assignRmVisitor (vname : string) = object(self)
  (* nopCilVisitor: walks the AST, without doing anything with it *)
  inherit nopCilVisitor 

  (* override the vinst method to do something with each instruction *)
  method vinst (i : instr) = 
    match i with
    | Set((Var vi, NoOffset), _, loc) when vi.vname = vname && vi.vglob ->
      E.log "%a: Assignment deleted: %a\n" d_loc loc d_instr i;
      ChangeTo []
    | _ -> SkipChildren 

end

(* <> = inquality operator *)
(* ignore = calls the function and ignores the return value *)
let processFunction ((tf, tv) : string * string) (fd : fundec) (loc : location) : unit =
  (* print function name *)
  E.log "%a: Processing function: %s\n" d_loc loc fd.svar.vname;
  if fd.svar.vname <> tf then () else begin
    let vis = new assignRmVisitor tv in
    ignore(visitCilFunction vis fd)
  end


(* tut2 = the entrypoint *)
(* funvar: targetFunName, varToRemove *)
(* iterGlobals will iterate over all globals in the file & call the function on each global *)
(* onlyFunctions will filter out all globals & only keep functions *)
(* processFunction will be called on each function *)
let tut2 (funvar : string * string) (f : file) : unit =
  funvar |> processFunction |> onlyFunctions |> iterGlobals f
  (* iterGlobals f (onlyFunctions (processFunction funvar)) *)

let () =
  let funvar = ("foo", "bar") in
  let f = Frontc.parse "../test/tut2.c" () in 
  tut2 funvar f;

  let out_channel = open_out "../test/tut2_modified.c" in
  Cil.dumpFile defaultCilPrinter out_channel "../test/tut2_modified.c" f;
  close_out out_channel
