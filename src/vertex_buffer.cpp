#include "vertex_buffer.h"

#include "glad/glad.h"

void vbo_init(struct VertexBuffer *vbo, const void *data, unsigned int size) {
    glGenBuffers(1, &vbo->id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void vbo_bind(struct VertexBuffer self) {
    glBindBuffer(GL_ARRAY_BUFFER, self.id);
}
