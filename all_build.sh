#!/bin/bash

copy_exec_files() {
    local src_dir="$1"
    local dst_dir="$2"

    # Ensure both arguments are provided
    if [ -z "$src_dir" ] || [ -z "$dst_dir" ]; then
        echo "Usage: copy_exec_files <source_dir> <destination_dir>"
        return 1
    fi

    # Ensure source exists and is a directory
    if [ ! -d "$src_dir" ]; then
        echo "Error: Source directory '$src_dir' does not exist."
        return 1
    fi

    mkdir -p "$dst_dir"

    # Copy only executable regular files (no recursion)
    for f in "$src_dir"/*; do
        if [ -f "$f" ] && [ -x "$f" ]; then
            cp -fv "$f" "$dst_dir"
        fi
    done
}


#clean
rm -rf build
rm -rf _deps

#build
cmake -S . -B build
cmake --build build -v

#package
rm -rf ./out && mkdir -p ./out


copy_exec_files "./build/main/" "./out"
copy_exec_files "./" "./out"

zip -r -9 ide_helper.zip ./out