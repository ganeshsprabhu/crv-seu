open Cil
open Pretty
open Myownciltut

(* open Tututil  *)

module L  = List     
module E  = Errormsg 

module IH = Inthash  
module DF = Dataflow  (* CIL's dataflow analysis module *)


let debug = ref false

(* 
  Top - The variable could be either odd or even.
  Even - The variable is an even integer.
  Odd - The variable is an odd integer.
  Bottom - The variable is uninitialized.
*)
type oekind = Top | Odd | Even | Bottom

(* type varmap = id * (varInfo * oekinfOfVar) *)
type varmap = int * (varinfo * oekind)

let id_of_vm   (vm : varmap) : int     = fst vm
let vi_of_vm   (vm : varmap) : varinfo = vm |> snd |> fst
let kind_of_vm (vm : varmap) : oekind  = vm |> snd |> snd


let string_of_oekind (k : oekind) : string =
  match k with
  | Top    -> "Top"
  | Odd    -> "Odd"
  | Even   -> "Even"
  | Bottom -> "Bottom"


let string_of_varmap (vm : varmap) : string =
  let vi = vi_of_vm vm in
  "("^vi.vname^", "^(vm |> kind_of_vm |> string_of_oekind)^")"


let string_of_varmap_list (vml : varmap list) : string =
  vml
  |> L.map string_of_varmap
  |> String.concat ", "


(* text comes from Pretty.ml *)
let varmap_list_pretty () (vml : varmap list) =
  vml |> string_of_varmap_list |> text

(* flip the oekind *)
let oekind_neg (k : oekind) : oekind =
  match k with
  | Even -> Odd
  | Odd -> Even
  | _ -> k

(* NOT INTERESTING *)
let varmap_equal (vm1 : varmap) (vm2 : varmap) : bool =
  (id_of_vm vm1) = (id_of_vm vm2) &&
  (kind_of_vm vm1) = (kind_of_vm vm2)


(* NOT INTERESTING *)
let varmap_list_equal (vml1 : varmap list) (vml2 : varmap list) : bool =
  let sort = L.sort (fun (id1,_) (id2,_) -> compare id1 id2) in
  list_equal varmap_equal (sort vml1) (sort vml2)


(* defines the partial order for our lattice *)
(* 
  Top > Bottom, Odd, Even
  Odd, Even > Bottom
  But Odd <> Even
*)
let oekind_includes (is_this : oekind) (in_this : oekind) : bool =
  match is_this, in_this with
  | _, Top -> true
  | Bottom, _ -> true
  | _, _ -> false


(* defines the least-upper-bound operation for our lattice *)
(* 
read like this:
  Any thing combined with Top gives Top
  Even combined with Odd gives Top
  Odd joined with itself or Bottom gives Odd likewise with Even
  Bottom combined with Bottom gives Bottom
 *)
let oekind_combine (k1 : oekind) (k2 : oekind) : oekind =
  match k1, k2 with
  | Top, _ | _, Top | Odd, Even | Even, Odd -> Top
  | Odd, _ | _, Odd -> Odd
  | Even, _ | _, Even -> Even
  | Bottom, Bottom -> Bottom

(* NOT INTERESTING *)
let varmap_combine (vm1 : varmap) (vm2 : varmap) : varmap option =
  match vm1, vm2 with
  | (id1, _), (id2, _) when id1 <> id2 -> None
  | (id1, (vi1, k1)), (_,(_,k2)) -> Some(id1,(vi1,oekind_combine k1 k2))

(* NOT INTERESTING *)
let varmap_list_combine_one (vml : varmap list) (vm : varmap) : varmap list =
  let id = id_of_vm vm in
  if L.mem_assoc id vml then
    let vm' = (id, L.assoc id vml) in
    let vm'' = forceOption (varmap_combine vm vm') in
    vm'' :: (L.remove_assoc (id_of_vm vm) vml)
  else vm :: vml

(* NOT INTERESTING *)
let varmap_list_combine (vml1 : varmap list) (vml2 : varmap list) : varmap list =
  L.fold_left varmap_list_combine_one vml1 vml2

(* NOT INTERESTING *)
let varmap_list_replace (vml : varmap list) (vm : varmap) : varmap list =
  vm :: (L.remove_assoc (id_of_vm vm) vml)

(* finds oekind of an int "i" - Odd or Even *)
let kind_of_int64 (i : Int64.t) : oekind =
  let firstbit = Int64.logand i Int64.one in
  if firstbit = Int64.one then Odd else Even

(* constFold will recursively evaluate an expression *)
let rec oekind_of_exp (vml : varmap list) (e : exp) : oekind =
  match e with
  | Const(CInt64(i, _, _)) -> kind_of_int64 i
  | Lval(Var vi, NoOffset) -> vml |> L.assoc vi.vid |> snd
  | SizeOf _ | SizeOfE _ | SizeOfStr _ | AlignOf _ | AlignOfE _ ->
    e |> constFold true |> oekind_of_exp vml
  | UnOp(uo, e, t) -> oekind_of_unop vml uo e
  | BinOp(bo, e1, e2, t) -> oekind_of_binop vml bo e1 e2
  | CastE(t, e) -> oekind_of_exp vml e
  | _ -> Top

(* finds oekind of a unary operator *)
and oekind_of_unop (vml : varmap list) (u : unop) (e : exp) : oekind =
  match u with
  | Neg  -> oekind_of_exp vml e
  | BNot -> e |> oekind_of_exp vml |> oekind_neg
  | LNot -> Top

(* finds oekind of a binary operator *)
and oekind_of_binop (vml : varmap list) (b : binop) (e1 : exp) (e2 : exp) : oekind =
  (* find out the oekind of the two expressions e1 & e2 first*)
  let k1, k2 = oekind_of_exp vml e1, oekind_of_exp vml e2 in
  match b with
  | PlusA -> begin
    match k1, k2 with
    (* Plus operator so: E+E = E, O+O = E, E+O = O, O+E = O *)
    | Even, Even -> Even
    | Odd, Odd -> Even
    | Even, Odd -> Odd
    | Odd, Even -> Odd
    | _, _ -> Top
  end
  
  | MinusA -> begin
    match k1, k2 with
    (* Minus operator so: E-E = E, O-O = E, E-O = O, O-E = O *)
    | Even, Even -> Even
    | Odd, Odd -> Even
    | Even, Odd -> Odd
    | Odd, Even -> Odd
    | _, _ -> Top
  end
  | Mult -> begin
    match k1, k2 with
    (* Multiplication operator so: E*E = E, O*O = O, E*O = E, O*E = E *)
    | Even, _ | _ , Even -> Even
    | Odd, Odd -> Odd
    | _, _ -> Top
  end
  
  
  | _ -> Top


(* if there's a write to memory of fn call, we must detroy anything we know about local var *)
(* moves the state of variables whose address has been taken to Top in resulting varmap *)
let varmap_list_kill (vml : varmap list) : varmap list =
  L.map (fun (vid, (vi, k)) ->
    if vi.vaddrof then (vid, (vi, Top)) else (vid, (vi, k)))
  vml


(* implement transfer fn for our dataflow analysis *)
(* looks at (instr, state) & calculates what state should be after instr in run *)
let varmap_list_handle_inst (i : instr) (vml : varmap list) : varmap list =
  match i with
  (* simple assignment to a local var: replace it's mapping in input state with oekind of RHS expr "e" *)
  | Set((Var vi, NoOffset), e, loc) when not(vi.vglob) && (isIntegralType vi.vtype) ->
    let k = oekind_of_exp vml e in
    varmap_list_replace vml (vi.vid,(vi,k)) 

  (* write to memory: we don't care for now *)
  | Set((Mem _, _), _, _) 

  (* fn call: kill all local vars, because we don't know what they're going to be *)
  | Call _ -> varmap_list_kill vml 
  | _ -> vml 


(* implements ForwardsTransfer signature & is the input module to DF.ForwardsDataFlow functor *)
(* functor = parameterized module -> think of DF.ForwardsDataFlow as a function that takes a module and returns a module *)
module OddEvenDF = struct

  let name = "OddEven"
  let debug = debug
  type t = varmap list
  let copy vml = vml
  let stmtStartData = IH.create 64
  let pretty = varmap_list_pretty
  let computeFirstPredecessor stm vml = vml


  let combinePredecessors (s : stmt) ~(old : t) (ll : t) =
    if varmap_list_equal old ll then None else
    Some(varmap_list_combine old ll)

  (* calls our transfer fn "varmap_list_handle_inst" on each instr in a stmt *)
  (* obtains new state after each instr & combines it with old state & then does a post-processing on it *)
  let doInstr (i : instr) (ll : t) =
    let action = varmap_list_handle_inst i in
    DF.Post action


  let doStmt stm ll = DF.SDefault
  let doGuard c ll = DF.GDefault
  let filterStmt stm = true

end


module OddEven = DF.ForwardsDataFlow(OddEvenDF)

(* collects all local vars of a function & sets their intial state to Bottom *)
let collectVars (fd : fundec) : varmap list =
  (fd.sformals @ fd.slocals)
  |> L.filter (fun vi -> isIntegralType vi.vtype)
  |> L.map (fun vi -> (vi.vid, (vi, Bottom)))

(* does the actual dataflow analysis *)
let computeOddEven (fd : fundec) : unit =
  (* first compute CFG of the function *)
  Cfg.clearCFGinfo fd;
  ignore(Cfg.cfgFun fd);

  (* get first stmt of the function *)
  let first_stmt = L.hd fd.sbody.bstmts in

  (* collect all local vars of the function *)
  let vml = collectVars fd in
  IH.clear OddEvenDF.stmtStartData;
  IH.add OddEvenDF.stmtStartData first_stmt.sid vml;

  (* do dataflow analysis *)
  OddEven.compute [first_stmt]


let getOddEvens (sid : int) : varmap list option =
  try Some(IH.find OddEvenDF.stmtStartData sid)
  with Not_found -> None

(* calculates the state of each local var after each instr in a stmt *)
let instrOddEvens (il : instr list) (vml : varmap list) : varmap list list =
  let proc_one hil i =
    match hil with
    | [] -> (varmap_list_handle_inst i vml) :: hil
    | vml':: rst as l -> (varmap_list_handle_inst i vml') :: l
  in
  il |> L.fold_left proc_one [vml]
     |> L.tl
     |> L.rev

class vmlVisitorClass = object(self)
  (* nopCilVisitor: walks the AST, without doing anything with it *)
  inherit nopCilVisitor

  val mutable sid           = -1
  val mutable state_list    = []
  val mutable current_state = None

  method vstmt stm =
    sid <- stm.sid;
    begin match getOddEvens sid with
    | None -> current_state <- None
    | Some vml -> begin
      match stm.skind with
      | Instr il ->
        current_state <- None;
        state_list <- instrOddEvens il vml
      | _ -> current_state <- None
    end end;
    DoChildren

  (* override the vinst method to do something with each instruction *)
  method vinst i =
    try let data = L.hd state_list in
        current_state <- Some(data);
        state_list <- L.tl state_list;
        DoChildren
    with Failure "hd" -> DoChildren

  method get_cur_vml () =
    match current_state with
    | None -> getOddEvens sid
    | Some vml -> Some vml

end


class varUseReporterClass = object(self)
  inherit vmlVisitorClass as super

  method vvrbl (vi : varinfo) =
    match self#get_cur_vml () with
    | None -> SkipChildren
    | Some vml -> begin
      if L.mem_assoc vi.vid vml then begin
        let vm = (vi.vid, L.assoc vi.vid vml) in
        E.log "%a: %a\n" d_loc (!currentLoc) varmap_list_pretty [vm]
      end;
      SkipChildren
    end

end

let evenOddAnalysis (fd : fundec) (loc : location) : unit =
  computeOddEven fd;
  let vis = ((new varUseReporterClass) :> nopCilVisitor) in
  ignore(visitCilFunction vis fd)

(* iterGlobals will iterate over all globals in the file & call the function on each global *)
(* onlyFunctions will filter out all globals & only keep functions *)
(* evenOddAnalysis will be called on each function *)
let tut3 (f : file) : unit =
  iterGlobals f (onlyFunctions evenOddAnalysis)

let () = 
  let f = Frontc.parse "../test/tut3.c" () in
  tut3 f;


  let out_channel = open_out "../test/tut3_modified.c" in
  Cil.dumpFile defaultCilPrinter out_channel "../test/tut3_modified.c" f;
  close_out out_channel
