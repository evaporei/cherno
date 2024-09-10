#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"

#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"
#include "texture.h"

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
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

    struct Texture texture = texture_init("res/textures/cherno_logo.png");

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
