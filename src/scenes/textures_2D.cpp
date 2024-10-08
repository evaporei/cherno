#include "textures_2D.h"

#include "imgui/imgui.h"

#include "../renderer.h"
#include "../texture.h"
#include "../vertex_buffer.h"
#include "../vertex_buffer_layout.h"
#include "../shader.h"

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
    vao_init(&scene->vao);

    struct VertexBuffer vbo;
    vbo_init(&vbo, positions, sizeof(positions));

    ibo_init(&scene->ibo, indices, sizeof(indices) / sizeof(indices[0]));

    struct Layout layout;
    layout_init(&layout);
    layout_push_f32(&layout, 2);
    layout_push_f32(&layout, 2);

    vao_add_buffer(&scene->vao, vbo, layout);

    shader_init(&scene->shader, "res/shaders/texture.vertex.shader", "res/shaders/texture.fragment.shader");

    struct Texture texture;
    texture_init(&texture, "res/textures/cherno_logo.png");

    // shader_set_uniform_4f(&scene->shader, "u_Color", 0.8, 0.3, 0.8, 1.0);
    shader_set_uniform_1i(&scene->shader, "u_Texture", 0);

    scene->name = "textures 2D scene";
    scene->translationA = glm::vec3(200, 200, 0);
    scene->translationB = glm::vec3(400, 200, 0);
}

void textures_2D_scene_update(struct Textures2DScene *self) {
    // empty for now
}

void textures_2D_scene_draw(struct Textures2DScene *self) {
    renderer_clear();

    shader_bind(&self->shader);

    {
        glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), self->translationA);

        glm::mat4 mvp = projection * view * model;
        shader_set_uniform_mat4f(&self->shader, "u_MVP", mvp);

        renderer_draw(&self->vao, &self->ibo);
    }

    {
        glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), self->translationB);

        glm::mat4 mvp = projection * view * model;
        shader_set_uniform_mat4f(&self->shader, "u_MVP", mvp);

        renderer_draw(&self->vao, &self->ibo);
    }
}

void textures_2D_scene_imgui(struct Textures2DScene *self) {
    ImGui::SliderFloat3("translation a", &self->translationA.x, 0.0f, 960.0f);
    ImGui::SliderFloat3("translation b", &self->translationB.x, 0.0f, 960.0f);
}
