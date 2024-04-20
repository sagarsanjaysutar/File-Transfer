#!/bin/sh

# A script file which will be run inside the docker container.

# Repo dir mounted to the docker container
repo_dir="/File-Transfer"

# Make build directory
mkdir -p ${repo_dir}/build

# Configre the project
cmake -B ${repo_dir}/build -S ${repo_dir}
if [ $? -eq 0 ]; then
    echo "CMake configuration successful."
else
    echo "CMake configuration failed."
fi

# Build the project
cmake --build ${repo_dir}/build
if [ $? -eq 0 ]; then
    echo "CMake building successful."
else
    echo "CMake building failed."
fi

# Run the app
/bin/bash -c "${repo_dir}/build/file_transfer"
if [ $? -eq 0 ]; then
    echo "Successful executed the app."
else
    echo "Failed to execute the app."
fi