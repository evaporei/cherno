#include "glad/glad.h"
#include "GLFW/glfw3.h"
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
    -0.5f, -0.5f,
    0.5, 0.5,
    0.5, -0.5,
    -0.5, 0.5
};

unsigned int indices[] = {
    0, 1, 2,
    0, 1, 3
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

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    const char *vertexShaderSrc = readFile("res/shaders/basic.vertex.shader");
    const char *fragmentShaderSrc = readFile("res/shaders/basic.fragment.shader");

    unsigned int program = createProgram(vertexShaderSrc, fragmentShaderSrc);

    glUseProgram(program);

    free((void*) vertexShaderSrc);
    free((void*) fragmentShaderSrc);

    int location = glGetUniformLocation(program, "u_Color");
    assert(location != -1);

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
