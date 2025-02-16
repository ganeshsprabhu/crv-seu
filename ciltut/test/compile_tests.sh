#!/bin/bash

# Remove all compiled object files : This removes only compiled executables and keeps the .c files
rm -f $(ls use_in_* | grep -v '\.c$')

# Compile all test cases
gcc -o use_in_array_index use_in_array_index.c
gcc -o use_in_bitwise_operation use_in_bitwise_operation.c
gcc -o use_in_compound_assignment use_in_compound_assignment.c
gcc -o use_in_expression use_in_expression.c
gcc -o use_in_for_loop use_in_for_loop.c
gcc -o use_in_function_argument use_in_function_argument.c
gcc -o use_in_if_condition use_in_if_condition.c
gcc -o use_in_logical_expression use_in_logical_expression.c
gcc -o use_in_macro_expansion use_in_macro_expansion.c
gcc -o use_in_pointer_dereferencing use_in_pointer_dereferencing.c
gcc -o use_in_return_statement use_in_return_statement.c
gcc -o use_in_struct_field_access use_in_struct_field_access.c
gcc -o use_in_switch_statement use_in_switch_statement.c
gcc -o use_in_ternary_operator use_in_ternary_operator.c
gcc -o use_in_while_loop use_in_while_loop.c

echo "Compilation complete for all test cases!"
