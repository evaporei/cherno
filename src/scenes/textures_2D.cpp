#include "textures_2D.h"

#include "imgui/imgui.h"

#include "../renderer.h"
#include "../texture.h"
#include "../vertex_buffer.h"
#include "../vertex_buffer_layout.h"

static float positions[] = {
    -50.0f, -50.0f, 0.0f, 0.0f,
    50.0f, -50.0f, 1.0f, 0.0f,
    50.0f, 50.0f, 1.0f, 1.0f,
    -50.0f, 50.0f, 0.0f, 1.0f
};

static unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

void textures_2D_scene_init(struct Textures2DScene *scene) {
    struct VertexArray vao = vao_init();

    struct VertexBuffer vbo = vbo_init(positions, sizeof(positions));

    struct IndexBuffer ibo = ibo_init(indices, sizeof(indices) / sizeof(indices[0]));

    struct Layout layout = layout_init();
    layout_push_f32(&layout, 2);
    layout_push_f32(&layout, 2);

    vao_add_buffer(&vao, vbo, layout);

    struct Shader shader = shader_init("res/shaders/texture.vertex.shader", "res/shaders/texture.fragment.shader");

    struct Texture _ = texture_init("res/textures/cherno_logo.png");

    shader_set_uniform_4f(&shader, "u_Color", 0.8, 0.3, 0.8, 1.0);
    shader_set_uniform_1i(&shader, "u_Texture", 0);

    scene->vao = vao;
    scene->ibo = ibo;
    scene->shader = shader;
    scene->translationA = glm::vec3(200, 200, 0);
    scene->translationB = glm::vec3(400, 200, 0);
}

void textures_2D_scene_update(struct Textures2DScene *self) {
    // empty for now
}

void textures_2D_scene_draw(struct Textures2DScene *self) {
    renderer_clear();

    {
        glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), self->translationA);

        glm::mat4 mvp = projection * view * model;
        shader_set_uniform_mat4f(&self->shader, "u_MVP", mvp);

        renderer_draw(&self->shader, &self->vao, &self->ibo);
    }

    {
        glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), self->translationB);

        glm::mat4 mvp = projection * view * model;
        shader_set_uniform_mat4f(&self->shader, "u_MVP", mvp);

        renderer_draw(&self->shader, &self->vao, &self->ibo);
    }
}

void textures_2D_scene_imgui(struct Textures2DScene *self) {
    ImGui::SliderFloat3("translation a", &self->translationA.x, 0.0f, 960.0f);
    ImGui::SliderFloat3("translation b", &self->translationB.x, 0.0f, 960.0f);
}
