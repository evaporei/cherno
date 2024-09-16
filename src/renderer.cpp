#include "renderer.h"

#include "glad/glad.h"
#include "error.h"

void renderer_clear() {
    glCall(glClear(GL_COLOR_BUFFER_BIT));
    glCall(glClearColor(0, 0, 0, 1));
}

void renderer_draw(struct VertexArray *vao, struct IndexBuffer *ibo) {
    vao_bind(vao);
    ibo_bind(ibo);
    glCall(glDrawElements(GL_TRIANGLES, ibo->count, GL_UNSIGNED_INT, 0));
}
