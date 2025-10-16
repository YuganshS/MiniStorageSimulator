#!/bin/bash

echo "Building Mini Storage Simulator..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

echo "Configuring with CMake..."
cmake ..

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

echo "Building project..."
cmake --build .

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build successful!"
echo "Executable location: build/bin/mini_storage_simulator"
echo "To run: ./build/bin/mini_storage_simulator"
