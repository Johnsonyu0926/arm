# Filename: build_and_copy_lib.sh
# Score: 97

#!/bin/bash

# Remove existing build directory and create a new one
rm -rf build
mkdir build
cd build

# Run cmake with Release build type and make
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Check if the build was successful
if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

# Create the destination directory if it doesn't exist
if [ ! -d "../../bin/lib_debug" ]; then
    mkdir -p ../../bin/lib_debug
fi

# Copy the built library to the destination directory
cp ./libmetartc7.a ../../bin/lib_debug/

# Check if the copy was successful
if [ $? -ne 0 ]; then
    echo "Failed to copy libmetartc7.a"
    exit 1
fi

echo "Build and copy completed successfully"

// By GST @Date