#pragma once

struct IndexBuffer {
    unsigned int id;
};

struct IndexBuffer ibo_init(const unsigned int *data, unsigned int count);
