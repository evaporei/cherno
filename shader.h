#pragma once

#include <unordered_map>

struct Shader {
    unsigned int id;
    std::unordered_map<const char*, int> uniformLocationCache;
};

struct Shader shader_init(const char* vertexShaderPath, const char* fragmentShaderPath);

void shader_bind(struct Shader self);

void shader_set_uniform_4f(struct Shader *self, const char *name, float v0, float v1, float v2, float v3);

void shader_set_uniform_1i(struct Shader *self, const char *name, int v);
