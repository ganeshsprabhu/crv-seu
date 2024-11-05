# crv-seu

# Steps for CIL

opam switch create 4.01.0
opam install frama-c
opam install cil

## Program slicing

frama-c t2.c -slice-value arr -then-on 'Slicing export' -print -ocode slice.c > res.txt
