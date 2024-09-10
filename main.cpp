#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <iostream>
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"

#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

struct Image {
    const unsigned char *data;
    unsigned long len;
    int width;
    int height;
};

struct Image newImage(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        std::cerr << "failed to open file: " << filepath << std::endl;
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

float positions[] = {
    -0.5, -0.5, 0.0, 0.0,
    0.5,  -0.5, 1.0, 0.0,
    0.5,  0.5,  1.0, 1.0,
    -0.5, 0.5,  0.0, 1.0
};

unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

void checkErrors() {
    unsigned int err = glGetError();
    while (err != GL_NO_ERROR) {
        printf("%d\n", err);
        err = glGetError();
    }
}

int main(void)
{
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "cherno opengl", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGL() == GL_FALSE)
    {
        return -1;
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    struct VertexArray vao = vao_init();

    struct VertexBuffer vbo = vbo_init(positions, sizeof(positions));

    int offset = 0;
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)offset
    );
    glEnableVertexAttribArray(0);
    offset += 2 * sizeof(float);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)offset
    );
    glEnableVertexAttribArray(1);

    struct IndexBuffer ibo = ibo_init(indices, sizeof(indices) / sizeof(indices[0]));

    struct Shader shader = shader_init("res/shaders/basic.vertex.shader", "res/shaders/basic.fragment.shader");

    struct Image img = newImage("res/textures/cherno_logo.png");
    if (!img.data)
        printf("no img from stb somehow mf\n");
    else
        printf("loaded img with %dx%d\n", img.width, img.height);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

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
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    free((void*) img.data);

    int location = glGetUniformLocation(shader.id, "u_Color");
    if (location == -1)
        printf("not using uniform u_Color\n");

    int texLocation = glGetUniformLocation(shader.id, "u_Texture");
    if (texLocation == -1)
        printf("not using uniform u_Texture\n");
    glUniform1i(texLocation, 0);

    checkErrors();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);

        // glUseProgram(program);
        // glBindVertexArray(vao);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        // glUniform4f(location, 0.8, 0.3, 0.8, 1.0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
