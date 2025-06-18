# Setting up required stuff for Catching CRV Variables using Frama-C and CIL (Only for C source code)
This repository contains the necessary steps to be able to run the concept as described in the paper for catching CRV variables for a C controller code.

Since the concept requires us to instrument the code (i.e., insert particular C code at certain places), we use Opam in order to slice the code (static analysis is done with respect to the output variable) and instrument the statements (before the use of the variable in consideration).

Note that the below instructions have all been performed and tested in WSL 2 terminal running on a windows 11 machine.

## Pre-Requisites
- Working Opam in your system
- C compiler
- OCaml compiler
Refer to online documentation to download the above.

## Setting Up Opam Switches
In order to have Frama-C and CIL, we create 2 separate Opam Switches (isolated environments to download OCaml packages)

### Frama-C Switch (v27.1)
Create a basic Opam Switch
```
opam switch create ocaml-frama-work 4.12.1
```
Sync to the created switch
```
eval $(opam env --switch=ocaml-frama-work --set-switch)
```
Load the dependencies as that in the 'frama-switch.export' file provided.
```
opam switch import ./frama-switch.export
```
If any errors occur regarding downloading some libraries in your local system, consider using the following command
```sudo apt install libexpat1-dev libgtk-3-dev libgtksourceview-3.0-dev```

Ensure that frama-c has been loaded in the switch by listing the version of frama-c.
```
opam list | grep frama-c
```
It should show an output similar to
```
"frama-c               27.1        Platform dedicated to the analysis of source code written in C"
```

### CIL Switch (v1.7.3)
Create a basic Opam Switch
```
opam switch create ocaml-cil-work 4.01.0
```
Sync to the created switch
```
eval $(opam env --switch=ocaml-cil-work --set-switch)
```
Load the dependencies as that in the 'cil-switch.export' file provided.
```
opam switch import ./cil-switch.export
```
Ensure that CIL has been loaded in the switch by listing the version of CIL
```
opam list | grep cil
```
It should show an output similar to
```
cil                 1.7.3       A front-end for the C programming language that facilitates program analysis and transformation
```

## Setting Up CBMS (v5.10)
Simply do the following command in the terminal
```
sudo apt install cbmc
```
Confirm the installation by running
```
cbmc testing_cbmc.c
```
It should show in the terminal a message showing the version of cbmc at the top and 'VERIFICATION SUCCESSFULL' at the end.
