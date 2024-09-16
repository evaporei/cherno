#include "index_buffer.h"

#include "glad/glad.h"

#include "error.h"

void ibo_init(struct IndexBuffer *ibo, const unsigned int *data, unsigned int count) {
    ibo->count = count;

    glCall(glGenBuffers(1, &ibo->id));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void ibo_bind(struct IndexBuffer *self) {
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->id));
}
