#include "vertex_buffer.h"

#include "glad/glad.h"

#include "error.h"

void vbo_init(struct VertexBuffer *vbo, const void *data, unsigned int size) {
    glCall(glGenBuffers(1, &vbo->id));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->id));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void vbo_bind(struct VertexBuffer self) {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, self.id));
}
