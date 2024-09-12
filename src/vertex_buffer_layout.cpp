#include "vertex_buffer_layout.h"

#include "glad/glad.h"

struct Layout layout_init() {
    struct Layout layout;

    layout.stride = 0;

    return layout;
}

void layout_push_f32(struct Layout *self, unsigned int count) {
    self->elements.push_back({ GL_FLOAT, count, GL_FALSE });
    self->stride += count * gl_size_of_type(GL_FLOAT);
}

void layout_push_u32(struct Layout *self, unsigned int count) {
    self->elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    self->stride += count * gl_size_of_type(GL_UNSIGNED_INT);
}

void layout_push_u8(struct Layout *self, unsigned int count) {
    self->elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    self->stride += count * gl_size_of_type(GL_UNSIGNED_BYTE);
}
