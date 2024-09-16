#pragma once

struct Texture {
    unsigned int id;
};

void texture_init(struct Texture *texture, const char* path);
