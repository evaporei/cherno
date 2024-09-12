#!/usr/bin/env bash

mkdir -p build
eval clang++ -arch arm64 -std=c++20 main.cpp vertex_array.cpp vertex_buffer.cpp vertex_buffer_element.cpp vertex_buffer_layout.cpp index_buffer.cpp renderer.cpp shader.cpp texture.cpp glad/src/glad.c $(pkg-config --libs --cflags glfw3) $(pkg-config --libs --cflags glm) -Iglad/include -framework OpenGL -o build/cherno
