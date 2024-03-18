#!/bin/bash

# Navigate to the serial_pagerank folder
cd serial_pagerank || { echo "Error: serial_pagerank folder not found"; exit 1; }

# Compile the C code
gcc -o power_method power_method.c matrix_generation.c links.c -lm

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    # Run the executable
    ./power_method
else
    echo "Compilation failed. Exiting..."
fi
