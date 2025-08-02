#!/bin/bash
#rm -rf ./build && mkdir build
#ln -s _deps ./build/_deps
rm -rf build
cmake -S . -B build -DCMAKE_EXE_LINKER_FLAGS="-static"
cmake --build build -v
