#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <iostream>
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

const char* readTextFile(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        std::cerr << "failed to open file: " << filepath << std::endl;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *contents = (char*) malloc(fileSize + 1);

    fread(contents, 1, fileSize, file);
    contents[fileSize] = '\0';

    fclose(file);

    return (const char*) contents;
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

unsigned int compileShader(unsigned int kind, const char *src)
{
    unsigned int shader = glCreateShader(kind);
    const int srcLen = (const int) strlen(src);
    glShaderSource(shader, 1, &src, &srcLen);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        char message[512];
        glGetShaderInfoLog(shader, strlen(message), NULL, message);
        std::cerr << "shader go bad, err: " << message << std::endl;
    }

    return shader;
}

unsigned int createProgram(const char *vsrc, const char *fsrc)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vsrc);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fsrc);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
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

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(1);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    const char *vertexShaderSrc = readTextFile("res/shaders/basic.vertex.shader");
    const char *fragmentShaderSrc = readTextFile("res/shaders/basic.fragment.shader");

    unsigned int program = createProgram(vertexShaderSrc, fragmentShaderSrc);

    glUseProgram(program);

    free((void*) vertexShaderSrc);
    free((void*) fragmentShaderSrc);

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
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    free((void*) img.data);

    int location = glGetUniformLocation(program, "u_Color");
    if (location == -1)
        printf("not using uniform u_Color\n");

    int texLocation = glGetUniformLocation(program, "u_Texture");
    if (texLocation == -1)
        printf("not using uniform u_Texture\n");
    glUniform1i(texLocation, 0);

    checkErrors();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);

        glUniform4f(location, 0.8, 0.3, 0.8, 1.0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
