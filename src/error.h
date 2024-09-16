#pragma once

#include "stdio.h"
#include "assert.h"

#include "glad/glad.h"

#ifdef DEBUG
    #define glCall(x) \
        glClearErrors(); \
        x; \
        assert(glCheckErrors(#x, __FILE__, __LINE__));
#else
    #define glCall(x) x 
#endif

void glClearErrors();

bool glCheckErrors(const char *function, const char *file, int line);
