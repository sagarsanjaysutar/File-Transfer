#!/bin/sh

# A script file which will be run inside the docker container.

# Location of repo inside the docker container
cd /File-Transfer

# Make build directory
mkdir -p build

# Configre the project
cmake -B build -S .
if [ $? -eq 0 ]; then
    echo "CMake configuration successful."
else
    echo "CMake configuration failed."
fi

# Build the project
cmake --build build
if [ $? -eq 0 ]; then
    echo "CMake building successful."
else
    echo "CMake building failed."
fi

# Run the app
/bin/bash -c "build/file_transfer"
if [ $? -eq 0 ]; then
    echo "Successful executed the app."
else
    echo "Failed to execute the app."
fi