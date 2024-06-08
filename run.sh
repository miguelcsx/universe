#!/bin/bash

# Function to check if a command is available
command_exists() {
  command -v "$1" >/dev/null 2>&1
}

# Check if the system is Ubuntu
if [ "$(uname -a | grep -o Ubuntu)" = "Ubuntu" ]; then
    echo "Ubuntu detected."
    if ! command_exists apt-get; then
        echo "This script requires apt-get which is not available on this system. Exiting..."
        exit 1
    fi
    # Install libglfw if not already installed
    if ! dpkg -l | grep -q libglfw3-dev; then
        echo "Installing libglfw..."
        sudo apt-get update
        sudo apt-get install -y libglfw3 libglfw3-dev
    else
        echo "libglfw already installed. Skipping..."
    fi
elif [ -n "$(command -v pacman)" ]; then
    echo "Pacman detected (Arch-based)."
    # Install libglfw if not already installed
    if ! pacman -Qs libglfw &>/dev/null; then
        echo "Installing libglfw..."
        sudo pacman -Syu libglfw-x11
    else
        echo "libglfw already installed. Skipping..."
    fi
elif [ -n "$(command -v dnf)" ]; then
    echo "DNF detected (Fedora-based)."
    # Install libglfw if not already installed
    if ! dnf list installed libglfw &>/dev/null; then
        echo "Installing libglfw..."
        sudo dnf install -y libglfw libglfw-devel
    else
        echo "libglfw already installed. Skipping..."
    fi
else
    echo "Unsupported distribution. Exiting..."
    exit 1
fi

# Check if OpenACC is available and install if necessary
if ! command_exists pgcc; then
    echo "OpenACC not detected."
    # Insert OpenACC installation steps here
else
    echo "OpenACC already installed. Skipping..."
fi

# Run CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Run Make
cmake --build build --config Release

# Run the program
./build/universe
