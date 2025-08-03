#!/bin/bash

#clean
rm -rf build
rm -rf _deps

#build
cmake -S . -B build
cmake --build build -v

#package
rm -rf ./out && mkdir -p ./out
cp -fv ./build/kernel_helper ./out
cp -fv ./build/atf_helper ./out
cp -fv ./build/merge ./out

cp -fv *gen_filelist.sh ./out
cp -fv test_all.sh ./out

zip -r -9 ide_helper.zip ./out