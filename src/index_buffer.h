#pragma once

struct IndexBuffer {
    unsigned int id;
    unsigned int count;
};

void ibo_init(struct IndexBuffer *ibo, const unsigned int *data, unsigned int count);

void ibo_bind(struct IndexBuffer self);
