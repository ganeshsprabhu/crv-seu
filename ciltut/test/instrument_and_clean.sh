#!/bin/bash

# List of test case names (without extensions)
test_cases=(
    "use_in_array_index"
    "use_in_bitwise_operation"
    "use_in_compound_assignment"
    "use_in_expression"
    "use_in_for_loop"
    "use_in_function_argument"
    "use_in_if_condition"
    "use_in_logical_expression"
    "use_in_macro_expansion"
    "use_in_pointer_dereferencing"
    "use_in_return_statement"
    "use_in_struct_field_access"
    "use_in_switch_statement"
    "use_in_ternary_operator"
    "use_in_while_loop"
)

# Path to the instrument_seu executable in the src directory
INSTRUMENT_EXEC="../src/instrument_seu"

# Iterate over test cases and process each
for file in "${test_cases[@]}"
do
    input_file="$file.c"
    output_file="${file}_output.c"
    clean_file="${file}_clean.c"

    echo "Processing $input_file..."
    $INSTRUMENT_EXEC "$input_file" "$output_file" x
    gcc -E -P "$output_file" -o "$clean_file"
done

echo "All test cases processed successfully!"
