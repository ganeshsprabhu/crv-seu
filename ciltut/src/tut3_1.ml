open Cil
open Pretty
open Myownciltut

(* Simplified Visitor to Add Print at Function Start *)
class printInserter (cilFile : file) = object
  inherit nopCilVisitor

  method! vfunc (fd : fundec) =
    Printf.printf "Processing function: %s\n" fd.svar.vname;
    let printfFunc =
      try findOrCreateFunc cilFile "printf" (TFun(voidType, Some ["fmt", charPtrType, []], true, []))
      with Not_found -> failwith "Failed to find or create printf function"
    in
    let printCall = mkStmt (Instr [Call(None, Lval(Var printfFunc, NoOffset), [Const (CStr "hello\n")], locUnknown)]) in
    fd.sbody.bstmts <- printCall :: fd.sbody.bstmts;
    Printf.printf "Added print at the start of function %s\n" fd.svar.vname;
    DoChildren
end

(* Function to process the file and apply the visitor *)
let processFile (f : file) =
  Printf.printf "Starting instrumentation to add prints at function starts\n";
  let vis = new printInserter f in
  visitCilFileSameGlobals vis f

(* Entry point for tut3.ml *)
let () =
  (* Hardcoded input and output file names *)
  let inputFile = "Poc_Program_Sliced.c" in
  let outputFile = "poc_program_modified.c" in

  (* Parse the input C file *)
  let cilFile = Frontc.parse inputFile () in

  (* Process the file to add print statements *)
  processFile cilFile;

  (* Write the modified file *)
  Printf.printf "Saving modified file to %s\n" outputFile;
  let outChannel = open_out outputFile in
  dumpFile defaultCilPrinter outChannel outputFile cilFile;
  close_out outChannel;

  Printf.printf "Instrumented file written to %s\n" outputFile
