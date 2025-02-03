#!/bin/bash

# Define build directory
BUILD_DIR="build"

# Ensure the build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

# Step 1: Run CMake to generate build system
echo "Running cmake to configure project..."
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug

# Step 2: Build the project
echo "Building the project..."
cmake --build "$BUILD_DIR"
