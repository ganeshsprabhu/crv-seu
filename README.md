# crv-seu

# Steps for CIL

opam switch create 4.01.0
opam install frama-c
opam install cil

## CFG Generation

dune exec -- frama-c t2.c

## Program slicing

frama-c t2.c -slice-value arr -then-on 'Slicing export' -print -ocode slice.c > res.txt

## EVA (evolved value anaylsis)

frama-c -eva t1.c -main abs

## Semantic Constant Folding

frama-c -semantic-const-folding t2.c -main f

NOTE: copy generated output to spare-code/t1.c

## Dead Code Elimination

frama-c -sparecode-analysis t1.c -main f

## Deadlock Detection

frama-c -deadlock t1.c -main main

# Steps for CILTUT

opam switch 4.01.0
eval $(opam env)

ocamlfind ocamlc -package cil -linkpkg -o tut2 myownciltut.ml tut2.ml

ocamlfind ocamlc -package cil -linkpkg -o tut3 myownciltut.ml tut3.ml

<!-- another terminal -->

cpp -P tut2_modified.c -o tut2_clean.c

cpp -P tut3_modified.c -o tut3_clean.c
