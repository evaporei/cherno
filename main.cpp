#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "stdlib.h"
#include "stdio.h"

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

const char* readFile(const char *filepath)
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

float positions[] = {
    -0.5f, -0.5f,
    0.5, 0.5,
    0.5, -0.5,
    -0.5, 0.5
};

unsigned int indices[] = {
    0, 1, 2,
    0, 1, 3
};

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

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    const char *vertexShaderSrc = readFile("res/shaders/basic.vertex.shader");

    std::cout << vertexShaderSrc << std::endl;

    free((void*) vertexShaderSrc);

    const char *fragmentShaderSrc = readFile("res/shaders/basic.fragment.shader");

    std::cout << fragmentShaderSrc << std::endl;

    free((void*) fragmentShaderSrc);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
