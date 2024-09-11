#pragma once

struct IndexBuffer {
    unsigned int id;
    unsigned int count;
};

struct IndexBuffer ibo_init(const unsigned int *data, unsigned int count);

void ibo_bind(struct IndexBuffer self);
