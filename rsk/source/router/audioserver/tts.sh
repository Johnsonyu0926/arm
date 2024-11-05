#!/bin/bash

# Script for Text-to-Speech conversion and playback

# Function to print usage information
print_usage() {
    echo "Usage: $0 <text>"
    echo "  <text> should be the text you want to convert to speech"
}

# Check if exactly one argument is provided
if [ "$#" -ne 1 ]; then
    print_usage
    exit 1
fi

TEXT="$1"
TTS_OUTPUT="/tmp/tts_output.wav"
TTS_COMMAND="tts --text \"$TEXT\" --out_path \"$TTS_OUTPUT\""

# Generate TTS audio
if ! eval "$TTS_COMMAND"; then
    echo "Error: Failed to generate TTS audio."
    exit 1
fi

# Play the generated audio
if ! ffplay -autoexit -nodisp -hide_banner -loglevel error "$TTS_OUTPUT"; then
    echo "Error: Failed to play the generated audio."
    rm -f "$TTS_OUTPUT"
    exit 1
fi

# Clean up
rm -f "$TTS_OUTPUT"

echo "TTS playback completed successfully."

#BY GST ARMV8 GCC 13.2