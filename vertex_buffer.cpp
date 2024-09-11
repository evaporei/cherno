#include "vertex_buffer.h"

#include "glad/glad.h"

struct VertexBuffer vbo_init(const void *data, unsigned int size) {
    struct VertexBuffer vbo;

    glGenBuffers(1, &vbo.id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return vbo;
}

void vbo_bind(struct VertexBuffer self) {
    glBindBuffer(GL_ARRAY_BUFFER, self.id);
}
