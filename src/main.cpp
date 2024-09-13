#include "assert.h"
#include "stdlib.h"
#include "stdio.h"

#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

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
    100.0f, 100.0f, 0.0f, 0.0f,
    200.0f, 100.0f, 1.0f, 0.0f,
    200.0f, 200.0f, 1.0f, 1.0f,
    100.0f, 200.0f, 0.0f, 1.0f
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

    GLFWwindow *window = glfwCreateWindow(960, 540, "cherno opengl", NULL, NULL);

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

    // "type of camera" -> ortho (flat/"2D") or perspective (depth/"3D")
    glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
    // "camera", to move to left, we must move everything to the right
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

    glm::vec3 translation(200, 200, 0);

    struct Shader shader = shader_init("res/shaders/basic.vertex.shader", "res/shaders/basic.fragment.shader");

    struct Texture _ = texture_init("res/textures/cherno_logo.png");

    shader_set_uniform_4f(&shader, "u_Color", 0.8, 0.3, 0.8, 1.0);
    shader_set_uniform_1i(&shader, "u_Texture", 0);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    // ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui_ImplOpenGL3_Init("#version 150");

    checkErrors();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        renderer_clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // translation, rotation and scale
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        
        // order matters because of OpenGL's column based ordering
        glm::mat4 mvp = projection * view * model;
        shader_set_uniform_mat4f(&shader, "u_MVP", mvp);

        renderer_draw(&shader, &vao, &ibo);

        {
            ImGui::SliderFloat3("translation", &translation.x, 0.0f, 960.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
