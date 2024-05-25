#!/bin/bash

# Run CMake
cmake -S . -B build

# Run Make
cmake --build build

# Run the program
./build/universe
