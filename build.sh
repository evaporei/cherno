#!/usr/bin/env bash

mkdir -p build
eval clang++ -arch arm64 -std=c++11 main.cpp vertex_array.cpp vertex_buffer.cpp vertex_buffer_element.cpp vertex_buffer_layout.cpp index_buffer.cpp shader.cpp texture.cpp glad/src/glad.c $(pkg-config --libs --cflags glfw3) -Iglad/include -framework OpenGL -o build/cherno
