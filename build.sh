#!/bin/bash

# Ensure we are in the right directory
echo "Running CMake and Ninja build..."

# go into build directory
cd build

# Run cmake with ninja generator
cmake -G "Ninja" ..

# Run ninja to build the project
ninja

./main
