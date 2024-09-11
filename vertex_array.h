#pragma once

// #include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

struct VertexArray {
    unsigned int id;
};

struct VertexArray vao_init();

void vao_add_buffer(struct VertexArray *self, struct Layout layout);
