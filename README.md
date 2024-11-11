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

frama-c -sparecode-analysis spare-code/t1.c -main f
