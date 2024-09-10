#pragma once

struct Texture {
    unsigned int id;
};

struct Texture texture_init(const char* path);
