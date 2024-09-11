#pragma once

struct VertexBuffer {
    unsigned int id;
};

struct VertexBuffer vbo_init(const void *data, unsigned int size);

void vbo_bind(struct VertexBuffer self);
