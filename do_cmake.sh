#!/bin/bash -e

# Create the build directory if it doesn't exist
test -d build || mkdir -p build

# Change into the build directory
cd build

# Run CMake to generate the build files
cmake ..

# Change back to the original directory
make

sudo make install
