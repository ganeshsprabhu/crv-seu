#!/bin/bash

ORIG_FILE="$1"      # e.g., cs1_org.c
SLICED_FILE="$2"    # e.g., cs1_sliced.c

if grep -q "int main" "$SLICED_FILE"; then
    echo "[INFO] main() already present in $SLICED_FILE. No action taken."
else
    echo "[INFO] main() not found. Extracting from $ORIG_FILE..."

    awk '/int main[[:space:]]*\(/,/^}/' "$ORIG_FILE" >> "$SLICED_FILE"

    echo "[SUCCESS] main() appended to $SLICED_FILE"
fi
