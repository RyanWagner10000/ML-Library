#!/bin/bash
# Create a new build folder and make the project

# Remove build folder if it exists
rm -rf build/

# Make new build folder
mkdir build

# cd into new build folder
cd build

# Make CMake project in build folder
cmake ..

# Make project
make
