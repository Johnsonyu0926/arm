#!/bin/bash

# Script to set the system volume level

# Function to print usage information
print_usage() {
    echo "Usage: $0 <volume-level>"
    echo "  <volume-level> should be an integer between 0 and 100"
}

# Check if exactly one argument is provided
if [ "$#" -ne 1 ]; then
    print_usage
    exit 1
fi

VOLUME_LEVEL="$1"

# Check if the input is a valid integer
if ! [[ "$VOLUME_LEVEL" =~ ^[0-9]+$ ]]; then
    echo "Error: Volume level must be a non-negative integer."
    print_usage
    exit 1
fi

# Check if the volume level is within the valid range
if [ "$VOLUME_LEVEL" -lt 0 ] || [ "$VOLUME_LEVEL" -gt 100 ]; then
    echo "Error: Volume level must be between 0 and 100."
    print_usage
    exit 1
fi

# Set the volume level
if amixer set Master "${VOLUME_LEVEL}%" > /dev/null 2>&1; then
    echo "Volume set to ${VOLUME_LEVEL}%"
else
    echo "Error: Failed to set volume. Make sure 'amixer' is installed and you have the necessary permissions."
    exit 1
fi

#BY GST ARMV8 GCC 13.2