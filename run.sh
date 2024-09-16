#!/usr/bin/env bash

set -e

RELEASE=$1

if [[ "$RELEASE" == "--release" ]] then
    cmake -S . -B release -DCMAKE_BUILD_TYPE=Release
    cmake --build release
    ./release/bin/cherno_exe
else
    # build/ folder is for debug mode
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    ./build/bin/cherno_exe
fi
