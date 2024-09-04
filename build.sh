#!/usr/bin/env bash

mkdir -p build
eval cc main.cpp $(pkg-config --libs --cflags glfw3) -o build/cherno
