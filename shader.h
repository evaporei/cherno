#pragma once

struct Shader {
    unsigned int id;
};

struct Shader shader_init(const char* vertexShaderPath, const char* fragmentShaderPath);
