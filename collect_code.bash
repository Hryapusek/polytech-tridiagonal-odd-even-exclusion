#!/bin/bash

# Check if a directory is provided as an argument
if [ -z "$1" ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

# Search for .cc and .hpp files in the given directory
find "$1" -type f \( -name "*.cc" -o -name "*.hpp" \) | while read -r file; do
    echo "===================================="
    echo "File: $file"
    echo "===================================="
    cat "$file"
    echo -e "\n"
done
