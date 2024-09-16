#include "error.h"

void glClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

// https://www.khronos.org/opengl/wiki/OpenGL_Error#Meaning_of_errors
bool glCheckErrors(const char *function, const char *file, int line) {
    unsigned int err = glGetError();
    while (err != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL ERROR!!\n");
        fprintf(stderr, "Code: 0x0%x\n", err);
        fprintf(stderr, "Message: ");
        switch (err) {
            case GL_INVALID_ENUM:
                fprintf(stderr, "enumeration parameter is not a legal enumeration for that function\n");
                break;
            case GL_INVALID_VALUE:
                fprintf(stderr, "value parameter is not a legal value for that function\n");
                break;
            case GL_INVALID_OPERATION:
                fprintf(stderr, "the set of state for a command is not legal for the parameters given to that command\n");
                break;
            case GL_STACK_OVERFLOW:
                fprintf(stderr, "stack pushing operation cannot be done because it would overflow the limit of that stack's size\n");
                break;
            case GL_STACK_UNDERFLOW:
                fprintf(stderr, "stack popping operation cannot be done because the stack is already at its lowest point\n");
                break;
            case GL_OUT_OF_MEMORY:
                fprintf(stderr, "performing an operation that can allocate memory, and the memory cannot be allocated\n");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                fprintf(stderr, "doing anything that would attempt to read from or write/render to a framebuffer that is not complete\n");
                break;
            case GL_CONTEXT_LOST:
                fprintf(stderr, "OpenGL context has been lost, due to a graphics card reset\n");
                break;
        }
        fprintf(stderr, "At: %s:%s:%d\n", function, file, line);
        return false;
        err = glGetError();
    }
    return true;
}
