#include "renderer.h"

#include "glad/glad.h"

void renderer_clear() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
}

void renderer_draw(struct Shader *shader, struct VertexArray *vao, struct IndexBuffer *ibo) {
    shader_bind(shader);
    vao_bind(vao);
    ibo_bind(ibo);
    glDrawElements(GL_TRIANGLES, ibo->count, GL_UNSIGNED_INT, 0);
}
