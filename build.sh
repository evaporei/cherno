#!/usr/bin/env bash

mkdir -p build
eval clang++ -arch arm64 -std=c++11 main.cpp vertex_array.c vertex_buffer.c index_buffer.c shader.c glad/src/glad.c $(pkg-config --libs --cflags glfw3) -Iglad/include -framework OpenGL -o build/cherno
