#!/bin/bash

# Convert audio files to desired format
SOURCE_DIR="/path/to/source"
DEST_DIR="/path/to/destination"
LOG_FILE="/var/log/audio_conversion.log"

# Function to convert audio files
convert_audio() {
    local src_file="$1"
    local dest_file="$2"
    ffmpeg -i "$src_file" -acodec mp3 -b:a 192k "$dest_file" -loglevel error
}

# Function to log messages
log_message() {
    local message="$1"
    echo "$(date +'%Y-%m-%d %H:%M:%S') - $message" >> "$LOG_FILE"
}

# Check if source directory exists
if [ ! -d "$SOURCE_DIR" ]; then
    log_message "Error: Source directory $SOURCE_DIR does not exist."
    exit 1
fi

# Create destination directory if it doesn't exist
mkdir -p "$DEST_DIR"

# Check if ffmpeg is installed
if ! command -v ffmpeg &> /dev/null; then
    log_message "Error: ffmpeg is not installed. Please install ffmpeg to continue."
    exit 1
fi

# Convert files
log_message "Starting audio conversion process."
for src_file in "$SOURCE_DIR"/*; do
    if [ -f "$src_file" ]; then
        base_name=$(basename "$src_file")
        dest_file="$DEST_DIR/${base_name%.*}.mp3"
        log_message "Converting $src_file to $dest_file"
        if convert_audio "$src_file" "$dest_file"; then
            log_message "Successfully converted $src_file"
        else
            log_message "Error converting $src_file"
        fi
    fi
done

log_message "All conversions completed."

# By GST ARMV8 GCC13.2 conv.sh