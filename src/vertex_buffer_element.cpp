#include "vertex_buffer_element.h"

#include "glad/glad.h"

int gl_size_of_type(unsigned int typ) {
    switch (typ) {
        case GL_FLOAT:
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        default:
            __builtin_unreachable();
    }
}
