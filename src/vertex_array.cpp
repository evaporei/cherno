#include "vertex_array.h"

#include "glad/glad.h"

void vao_init(struct VertexArray *vao) {
    glGenVertexArrays(1, &vao->id);
    glBindVertexArray(vao->id);
}

void vao_bind(struct VertexArray *self) {
    glBindVertexArray(self->id);
}

void vao_add_buffer(struct VertexArray *self, struct VertexBuffer vbo, struct Layout layout) {
    vbo_bind(vbo);
    int offset = 0;
    for (unsigned int i = 0; i < layout.elements.size(); i++) {
        struct Element *element = &layout.elements[i];
        glVertexAttribPointer(
            i,
            2,
            element->typ,
            element->normalized,
            layout.stride,
            (const void*)offset
        );
        glEnableVertexAttribArray(i);
        offset += element->count * gl_size_of_type(element->typ);
    }
}
