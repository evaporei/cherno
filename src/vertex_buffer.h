#pragma once

struct VertexBuffer {
    unsigned int id;
};

void vbo_init(struct VertexBuffer *vbo, const void *data, unsigned int size);

void vbo_bind(struct VertexBuffer self);
