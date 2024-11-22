open Cil
open Pretty
open Myownciltut

module L = List

type functions = {
  mutable begin_loop : varinfo;
  mutable end_loop   : varinfo;
  }
  
  
(* dummy "varinfo" that we can use to initialize the functions record*)
(* makeVarinfo : bool -> string -> typ -> varinfo *)
(* makeVarinfo : isGlobal -> fnName -> returnType -> varinfo *)
let dummyVar = makeVarinfo false "_tut_foo" voidType

(* instance of record type "functions" *)
(* let's us quickly refer to "varinfos" for our instrumentation functions *)
let tutfuns = {
  begin_loop = dummyVar;
  end_loop   = dummyVar;
}

(* variables names of our instrumentation functions *)
let begin_loop_str = "tut_begin_loop"
let end_loop_str   = "tut_end_loop"

(* we might be looping multiple times & want to ignore calls to instrumentation functions that we've added *)
(* define list of names of our instrumentation functions *)
let tut_function_names = [
  begin_loop_str;
  end_loop_str;
  ]
  
(* isTutFun will check if a function name is one of our instrumentation functions *)
let isTutFun (name : string) : bool =
  (* L.mem : 'a -> 'a list -> bool = <fun> *)
  L.mem name tut_function_names

(* mkFunTyp is a helper fn for making simple function types *)
(* For example:
   double f(int x, float y) { return 2.1; } 
   // "double"                = rt 
   // [(x, int); (y, float)]  = args (argument list of tuples (varName, typ))
   // "f"                     = name 
*)
let mkFunTyp (rt : typ) (args : (string * typ) list) : typ =
  TFun(rt, Some(L.map (fun a -> (fst a, snd a, [])) args), false, [])
  (* TFun of typ * (string * typ * attributes) list option * bool * attributes *)

(* initialize our "tutfuns" record with dummy varinfos *)
let initTutFunctions (f : file) : unit =
  (* findOrCreateFunc : if a fn is not found a prototype is added to the file, varinfo returned *)
  let focf : string -> typ -> varinfo = findOrCreateFunc f in

  (*
    void tut_begin_loop(const char *f, int l);
    void tut_end_loop(const char *f, int l, int c);
  *)
  let bl_type = mkFunTyp voidType ["f", charConstPtrType; "l", intType] in
  let el_type = mkFunTyp voidType ["f", charConstPtrType; "l", intType; "c", intType;] in
  
  (* create x2 new functions & store their varinfos in our "tutfuns" record *)
  tutfuns.begin_loop <- focf begin_loop_str bl_type;
  tutfuns.end_loop   <- focf end_loop_str   el_type

(* creates x4 new statements that we'll insert into the body of a loop *)
(* i2s : instr -> stmt *)
let makeInstrStmts (counter : varinfo) (loc : location)
: stmt * stmt * stmt * stmt =
  (* f, l are inputs to the instrumentation functions *)
  (* f: char const *, l: int *)
  (* mkString turns an OCaml string into a Cil.exp, similarly integer fn does the same for OCaml ints *)
  (* right now default input to the function is the file name and line number *)
  let f, l = mkString loc.file, integer loc.line in

  (* calls the "tut_begin_loop" instrumentation function *)
  (* Call of lval option * exp * exp list * location *)
  (* v2e creates an exp from a varinfo *)
  i2s (Call(None, v2e tutfuns.begin_loop, [f; l], loc)),

  (* sets the loop counter to 0 *)
  (* Set of lval * exp * location	 *)
  (* var fn creates a lval from a varinfo *)
  i2s (Set(var counter, zero, loc)),

  (* increments the loop counter *)
  i2s (Set(var counter,BinOp(PlusA, v2e counter, one, counter.vtype), loc)),

  (* calls the "tut_end_loop" instrumentation function *)
  i2s (Call(None, v2e tutfuns.end_loop, [f; l; v2e counter], loc))



class loopInstrumenterClass (fd : fundec) = object(self)
  (* nopCilVisitor: walks the AST, without doing anything with it *)
  inherit nopCilVisitor

  (* override the vstmt method to do something with each statement *)
  method vstmt (s : stmt) =
    let action s =
      match s.skind with
      (* Loop of block * location * stmt option * stmt option *)
      | Loop(b, loc, co, bo) ->
        let counter = makeTempVar fd intType in
        (* 
          ss:  call to "tut_begin_loop"
          cis: set the loop counter to 0
          is:  increment the loop counter
          es:  call to "tut_end_loop"
        *)
        let ss, cis, is, es = makeInstrStmts counter loc in

        (* add the increment counter stmt to the loop body *)
        (* 'a :: 'a list -> 'a list *)
        b.bstmts <- is :: b.bstmts;

        (* make a new block with the stmts *)
        (* mkBlock : stmt list -> block *)
        (* 
          ss:             call to "tut_begin_loop"
          cis:            set the loop counter to 0
          mkStmt s.skind: actual loop stmt
          es:             call to "tut_end_loop"
        *)
        let nb = mkBlock [ss; cis; mkStmt s.skind; es] in
        s.skind <- Block nb;
        s
      | _ -> s
    in
    (* we can have multiple nested loops, so we recurse into s's children before handling s *)
    ChangeDoChildrenPost(s, action)

end

(* processFunction : applies the loopInstrumenterClass to each function in the file *)
let processFunction (fd : fundec) (loc : location) : unit =
  let vis = new loopInstrumenterClass fd in
  ignore(visitCilFunction vis fd)


let tut4 (f : file) : unit =
  (* add prototype definitions for our instrumentation functions *)
  initTutFunctions f;

  (* apply the loopInstrumenterClass to each function in the file *)
  iterGlobals f (onlyFunctions processFunction)

let () = 
  let f = Frontc.parse "../test/tut4.c" () in
  tut4 f;

  (* print the modified AST *)
  let out_channel = open_out "../test/tut4_modified.c" in
  Cil.dumpFile defaultCilPrinter out_channel "../test/tut4_modified.c" f;
  close_out out_channel



