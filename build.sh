#!/usr/bin/env bash

mkdir -p build
eval cc main.cpp glad/src/glad.c $(pkg-config --libs --cflags glfw3) -Iglad/include -framework OpenGL -o build/cherno
