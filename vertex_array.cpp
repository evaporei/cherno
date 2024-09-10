#include "vertex_array.h"

#include "glad/glad.h"

struct VertexArray vao_init() {
    struct VertexArray vao;
    glGenVertexArrays(1, &vao.id);
    glBindVertexArray(vao.id);
    return vao;
}
