#include "index_buffer.h"

#include "glad/glad.h"

struct IndexBuffer ibo_init(const unsigned int *data, unsigned int count) {
    struct IndexBuffer ibo;
    ibo.count = count;

    glGenBuffers(1, &ibo.id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

    return ibo;
}

void ibo_bind(struct IndexBuffer self) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self.id);
}
