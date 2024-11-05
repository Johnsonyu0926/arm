# dodownload.sh
#!/bin/bash

# Script to download content from a URL and save it to a file
# Usage: ./script_name.sh <URL> <Output File>

# Function to log messages
log_message() {
    echo "$(date +'%Y-%m-%d %H:%M:%S') - $1"
}

# Check if correct number of arguments are provided
if [ "$#" -ne 2 ]; then
    log_message "Error: Incorrect number of arguments."
    echo "Usage: $0 <URL> <Output File>"
    exit 1
fi

URL="$1"
OUTPUT_FILE="$2"

# Check if curl is installed
if ! command -v curl &> /dev/null; then
    log_message "Error: curl is not installed. Please install curl to continue."
    exit 1
fi

# Attempt to download the file
log_message "Starting download from $URL"
if curl -A "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36" \
        "$URL" -L -o "$OUTPUT_FILE" --fail --silent --show-error; then
    log_message "Download completed successfully. Saved to $OUTPUT_FILE"
else
    log_message "Error: Download failed. Please check the URL and try again."
    exit 1
fi

# By GST ARMV8 GCC13.2 download_url.sh

/*
/*使用方法保持不变：
/*./script_name.sh <URL> <Output File>

/*请确保给予脚本执行权限：
/*chmod +x script_name.sh