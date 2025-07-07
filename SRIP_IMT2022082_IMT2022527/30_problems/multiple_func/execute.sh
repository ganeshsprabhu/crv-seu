#!/bin/bash

# Create output directory for executables
mkdir -p bin

# Compile all .c files in the current directory, excluding header files
for file in *.c; do
    # Get the base filename without extension and clean it of special characters
    base=$(basename "$file" .c)
    sanitized=$(echo "$base" | tr -cd '[:alnum:]_')

    # Compile the C file to an executable
    gcc "$file" -o "bin/${sanitized}" 2> /dev/null -lm

    # Check if compilation was successful
    if [ $? -eq 0 ]; then
        echo "Compiled $file -> bin/${sanitized}"
    else
        echo "Failed to compile $file"
    fi
done
