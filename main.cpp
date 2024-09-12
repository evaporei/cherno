#include "assert.h"
#include "stdlib.h"
#include "stdio.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "index_buffer.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
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

int main(void) {
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "cherno opengl", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGL() == GL_FALSE) {
        return -1;
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    struct VertexArray vao = vao_init();

    struct VertexBuffer vbo = vbo_init(positions, sizeof(positions));

    struct IndexBuffer ibo = ibo_init(indices, sizeof(indices) / sizeof(indices[0]));

    struct Layout layout = layout_init();
    layout_push_f32(&layout, 2);
    layout_push_f32(&layout, 2);

    vao_add_buffer(&vao, vbo, layout);

    glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

    struct Shader shader = shader_init("res/shaders/basic.vertex.shader", "res/shaders/basic.fragment.shader");

    struct Texture texture = texture_init("res/textures/cherno_logo.png");

    shader_set_uniform_4f(&shader, "u_Color", 0.8, 0.3, 0.8, 1.0);
    shader_set_uniform_1i(&shader, "u_Texture", 0);
    shader_set_uniform_mat4f(&shader, "u_MVP", projection);

    checkErrors();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        renderer_clear();

        renderer_draw(&shader, &vao, &ibo);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
