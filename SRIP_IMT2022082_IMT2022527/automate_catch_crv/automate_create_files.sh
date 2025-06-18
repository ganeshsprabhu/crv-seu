#!/bin/bash

source_file=""					#Input source file path.
entry_func=""					#The function from which the static analysis is meant to be done.
entry_variable=""				#The variable inside the entry function whose independent variables are to be removed.

sliced_file=""					#Contains sliced code from source file.
sliced_instrumented_file=""			#Uses sliced_file and inserts the 'simulate_seu' statements.
crv_check_variable=""				#The variable for which you would like to check it's conditional relevance.
sliced_instrumented_clean_file=""		#Uses instrumented file and cleans the code of comments and unnecessary code.
sliced_instrumented_clean_renamed=""		#Uses the cleaned code and renames the function name by adding '_prime_${variable}' to the function name under consideration.
final_output_file=""				#Uses original source file and the instrumented file to put them together and evaluate their outputs.

echo "[+] Switching to Frama-C OPAM switch..."
eval $(opam env --switch=ocaml-frama-work --set-switch)

echo "Source file path to check for CRVs"
read source_file
filename="${source_file%.c}"
sliced_file="${filename}_sliced.c"
sliced_instrumented_file="${filename}_instru.c"
sliced_instrumented_clean_file="${filename}_instru_clean.c"
sliced_instrumented_clean_renamed="${filename}_instru_clean_renamed.c"
final_output_file="${filename}_cbmc_ready.c"

echo "Enter the entry point function and the variable you would like to slice for"
read entry_function
read entry_variable

frama-c -load-module slicing "$source_file" -main "$entry_function" -slice-value "${entry_variable}" -slicing-level 3 -then-on 'Slicing export' -print -ocode "${sliced_file}"
echo "Finished static analysis on ${source_file} file. The sliced code is now available in file ${sliced_file}"

echo "[+] Switching to CIL OPAM switch..."
eval $(opam env --switch=ocaml-cil-work --set-switch)

echo "Ensure that 'instrument_seu' executable is present in the current working directory"
echo "What variable would you like to check the instrumentation of"
read variable

./instrument_seu "${sliced_file}" "${sliced_instrumented_file}" "${variable}"

echo "Finished preparing instrumented code. Available in ${sliced_instrumented_file} file. Cleaning it now."
gcc -E -P "${sliced_instrumented_file}" -o "${sliced_instrumented_clean_file}"

echo "Instrumented function now present in ${sliced_instrumented_clean_file}"

echo "Moving on to modify the cleaned code by adding '_prime_${variable}' to the function '${entry_function}'"
new_func_name="${entry_function}_prime_${variable}"
sed "s/\b${entry_function}\b/${new_func_name}/g" "$sliced_instrumented_clean_file" > "$sliced_instrumented_clean_renamed"

echo "Finished renaming the function. Saved to ${sliced_instrumented_clean_renamed}"

echo "Creating the Final C file for testing the concept"
cp "${source_file}" "${final_output_file}"
echo -e "\n\n// ----- Renamed Instrumented Function -----\n" >> "${final_output_file}"
sed -i 's/\r$//' "$final_output_file"
cat "$sliced_instrumented_clean_renamed" >> "$final_output_file"

echo "[+] Combined file created: $final_output_file"
