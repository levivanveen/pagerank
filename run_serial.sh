#!/bin/bash

SRC_DIR="serial_pagerank"
INCLUDE_DIR="include"

# Compile the C code
gcc -o "$SRC_DIR/power_method" "$SRC_DIR/power_method.c" "$SRC_DIR/matrix_generation.c" "$SRC_DIR/links.c" -lm -I "$INCLUDE_DIR"

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    cd "$SRC_DIR"
    # Run the executable
    ./power_method
else
    echo "Compilation failed. Exiting..."
fi
