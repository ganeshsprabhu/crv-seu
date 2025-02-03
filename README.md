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

ocamlfind ocamlc -package cil -linkpkg -o tut4 myownciltut.ml tut4.ml

ocamlfind ocamlc -package cil -linkpkg -o Poc_Program_Sliced_Instrument myownciltut.ml Poc_Program_Sliced_Instrument.ml

<!-- another terminal -->

cpp -P tut2_modified.c -o tut2_clean.c

cpp -P tut3_modified.c -o tut3_clean.c

cpp -P tut4_modified.c -o tut4_clean.c

cpp -P Poc_Program_Sliced_Instrument_modified.c -o Poc_Program_Sliced_Instrument_clean.c

# Run in docker

```bash
# run container and mount volume
docker run --name ciltut -it -d -v ./ciltut:/home/opam/app rohitshah1706/ciltut:latest

# attach container via CLI or you could also use vscode extension to attach to container
docker exec -it ciltut /bin/bash
```

# Updated Execution steps
```bash
iiitb@iiitb-vm:~/crv-seu/ciltut$ docker exec -it ciltut /bin/bash
e8209c293a7f:~/app$ ls
Dockerfile  src  test
e8209c293a7f:~/app$ cd src/
e8209c293a7f:~/app/src$ 
eval $(opam env)
ocamlfind ocamlc -package cil -linkpkg -o instrument_seu  myownciltut.ml instrument_seu.ml
./instrument_seu
cpp -P output.c -o output_1.c
```bash
