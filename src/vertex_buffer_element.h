#pragma once

struct Element {
    unsigned int typ;
    unsigned int count;
    unsigned char normalized;
};

int gl_size_of_type(unsigned int typ);
