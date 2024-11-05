# Filename: build_script.sh
# Score: 95

#!/bin/bash

# Remove existing build directory
rm -rf build

# Create a new build directory
mkdir build

# Navigate to the build directory
cd build

# Set the architecture
ARCH=aarch64.cmake

# Run cmake with the specified toolchain file and build type
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../$ARCH ..

# Compile the project
make
# By GST @Date