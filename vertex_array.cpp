#include "vertex_array.h"

#include "glad/glad.h"

struct VertexArray vao_init() {
    struct VertexArray vao;
    glGenVertexArrays(1, &vao.id);
    glBindVertexArray(vao.id);
    return vao;
}

void vao_add_buffer(struct VertexArray *self, struct Layout layout) {
    int offset = 0;
    for (unsigned int i = 0; i < layout.elements.size(); i++) {
        struct Element element = layout.elements[i];
        glVertexAttribPointer(
            i,
            2,
            element.typ,
            element.normalized,
            layout.stride,
            (const void*)offset
        );
        glEnableVertexAttribArray(i);
        offset += element.count * gl_size_of_type(element.typ);
    }
}
