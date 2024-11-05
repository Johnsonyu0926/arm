# Score: 95

#!/bin/bash

# Navigate to libmetartccore7 directory and run cmake script
cd libmetartccore7
./cmake_x64.sh

# Check if the previous command was successful
if [ $? -ne 0 ]; then
    echo "Failed to build libmetartccore7"
    exit 1
fi

# Navigate to libmetartc7 directory and run cmake script
cd ../libmetartc7
./cmake_x64.sh

# Check if the previous command was successful
if [ $? -ne 0 ]; then
    echo "Failed to build libmetartc7"
    exit 1
fi

echo "Build completed successfully"

// By GST @Date