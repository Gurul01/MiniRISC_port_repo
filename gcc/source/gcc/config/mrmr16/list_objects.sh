#!/bin/bash
dir="$1"
if [ -d "$dir" ]; then
    # Use find to list full paths of .o files
    find "$dir" -maxdepth 1 -type f -name '*.o' -exec realpath {} \;
else
    echo "Directory $dir does not exist" >&2
    exit 1
fi
