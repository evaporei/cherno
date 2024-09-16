#include "texture.h"

#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include "error.h"

struct Image {
    const unsigned char *data;
    unsigned long len;
    int width;
    int height;
};

void image_init(struct Image *image, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "failed to open file: %s\n", filepath);
        image->data = NULL;
        image->len = 0;
        image->width = 0;
        image->height = 0;
        return;
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

    image->data = data;
    image->len = (unsigned long) fileSize;
    image->width = width;
    image->height = height;
}

void texture_init(struct Texture *texture, const char* path) {
    struct Image img;
    image_init(&img, path);
    if (!img.data)
        printf("no img from stb somehow mf\n");
    else
        printf("loaded img with %dx%d\n", img.width, img.height);

    glCall(glGenTextures(1, &texture->id));
    glCall(glBindTexture(GL_TEXTURE_2D, texture->id));

    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    glCall(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        img.width,
        img.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        img.data
    ));
    glCall(glBindTexture(GL_TEXTURE_2D, texture->id));
    glCall(glActiveTexture(GL_TEXTURE0 + 0));
    glCall(glBindTexture(GL_TEXTURE_2D, texture->id));

    free((void*) img.data);
}
