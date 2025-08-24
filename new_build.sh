#!/bin/bash
# Create a new build folder and make the project

function try()
{
    [[ $- = *e* ]]; SAVED_OPT_E=$?
    set +e
}

function throw()
{
    exit $1
}

function catch()
{
    export exception_code=$?
    (( $SAVED_OPT_E )) && set +e
    return $exception_code
}

# Clear terminal
clear

# Remove build folder if it exists
rm -rf build/

# Make new build folder, but don't error if it already exists
mkdir -p build

# cd into new build folder
cd build

# Try to Make CMake project in build folder
try
(
    cmake ..
)
catch || {
    echo "'cmake ..' operation was unsuccessful."
    exit 1
}

# Try to Make the project
try
(
    make -j4
)
catch || {
    echo "'make -j4' operation was unsuccessful."
    exit 1
}

# Sources
# https://www.xmodulo.com/catch-handle-errors-bash.html