#!/bin/bash

# Run CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Run Make
cmake --build build --config Release

# Run the program
./build/universe
