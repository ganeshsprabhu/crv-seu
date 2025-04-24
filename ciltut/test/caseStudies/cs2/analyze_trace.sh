#!/bin/bash

# File containing safety condition (e.g., "output <= 10")
safety_condition_file="safety_condition.txt"

# File containing the trace data
trace_file="simulate_seu_trace.txt"

# Extract unique x values from the trace file and write to a temporary file
grep -oP '(?<![a-zA-Z0-9_])humidity_effect=\d+' "$trace_file" | sort | uniq > crv_values.txt

# Read the safety condition from the file
safety_condition=$(<"$safety_condition_file")

# Clear the previous safety results (optional, if needed)
> crv_values_results.txt

# Iterate over each x value from the temporary file
while read -r x_val; do
    # Extract the numeric part of x
    num_value=$(echo "$x_val" | grep -oP '\d+')

    # Perform a test based on the safety condition
    # For example, if the safety condition is "output <= 10"
    if (( num_value <= 300 )); then
        # Satisfies the safety condition
        echo "$x_val;satisfies" >> crv_values_results.txt
    else
        # Violates the safety condition
        echo "$x_val;violates" >> crv_values_results.txt
    fi
done < crv_values.txt

# Print the results (optional, if you want to display the content of the result)
cat crv_values_results.txt
