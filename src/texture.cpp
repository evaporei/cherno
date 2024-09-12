#include "texture.h"

#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

struct Image {
    const unsigned char *data;
    unsigned long len;
    int width;
    int height;
};

struct Image newImage(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "failed to open file: %s\n", filepath);
        return { .data = NULL, .len = 0, .width = 0, .height = 0 };
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *contents = (char*) malloc(fileSize);

    fread(contents, 1, fileSize, file);

    fclose(file);

    int width, height;

    stbi_set_flip_vertically_on_load(1);
    int desiredChannels = 3;
    const unsigned char* data = stbi_load_from_memory((const unsigned char*)contents, fileSize, &width, &height, &desiredChannels, 4);

    free((void*) contents);

    return { .data = data, .len = (unsigned long) fileSize, .width = width, .height = height };
}

struct Texture texture_init(const char* path) {
    struct Image img = newImage(path);
    if (!img.data)
        printf("no img from stb somehow mf\n");
    else
        printf("loaded img with %dx%d\n", img.width, img.height);

    struct Texture texture;
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        img.width,
        img.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        img.data
    );
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    free((void*) img.data);

    return texture;
}
