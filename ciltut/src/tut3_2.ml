open Cil
open Pretty
open Myownciltut

(* Simplified Visitor to Insert Print Before Every Use of 'y' *)
class useInserter (cilFile : file) = object
  inherit nopCilVisitor

  method! vexpr (e : exp) =
    match e with
    | Lval (Var vi, NoOffset) when vi.vname = "y" ->
      Printf.printf "Found use of variable 'y', inserting print statement\n";
      let printfFunc =
        try findOrCreateFunc cilFile "printf" (TFun(voidType, Some ["fmt", charPtrType, []], true, []))
        with Not_found -> failwith "Failed to find or create printf function"
      in
      let printCall = Call(None, Lval(Var printfFunc, NoOffset), [Const (CStr "hello\n")], locUnknown) in
      CompoundInit(TVoid [], [Field((Fieldinfo.create "__cil__dummy", TVoid [], [])), SingleInit (Const(CInt64(0L, IUInt, None)))]) |> ignore;
      ChangeTo (Lval(Var vi, NoOffset));
      ChangeTo (CastE(TVoid [], Const (CStr "hello\n")))
    | _ ->
      DoChildren
end

(* Function to process the file and apply the visitor *)
let processFile (f : file) =
  Printf.printf "Starting instrumentation to add prints before uses of 'y'\n";
  let vis = new useInserter f in
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
