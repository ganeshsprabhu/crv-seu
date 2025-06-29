#!/bin/bash
set -e
opam init --comp=4.12.1 --disable-sandboxing -a
# Setup Frama-C switch
echo "=== Setting up Frama-C switch ==="
opam switch create ocaml-frama-work 4.12.1
eval $(opam env --switch=ocaml-frama-work --set-switch)
opam switch import ./frama-switch.export

echo "Verifying Frama-C installation:"
if opam list | grep -q frama-c; then
    echo "frama-c installed successfully."
else
    echo "frama-c not found!"; exit 1;
fi

echo "=== Setting up Cil ==="
opam switch create ocaml-cil-work 4.01.0
eval $(opam env --switch=ocaml-cil-work --set-switch)
echo "Adding github archive repository for OCaml packages"
opam repo add archive https://github.com/ocaml/opam-repository-archive.git
opam update
echo "Installing CIL and its dependencies"
opam switch import ./cil-switch.export
echo "Verifying CIL installation:"
if opam list | grep -q cil; then
    echo "CIL installed successfully."
else
    echo "CIL not found!"; exit 1;
fi

# Verify CBMC
echo "Verifying CBMC:"
if cbmc --version > /dev/null 2>&1; then
    cbmc --version
    echo "CBMC installed successfully."
else
    echo "CBMC not installed correctly"; exit 1;
fi
