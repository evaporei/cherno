#pragma once

#include "vertex_array.h"
#include "index_buffer.h"

void renderer_clear();

void renderer_draw(struct VertexArray *vao, struct IndexBuffer *vbo);
