#pragma once

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

void renderer_clear();

void renderer_draw(struct Shader *shader, struct VertexArray *vao, struct IndexBuffer *vbo);
