#include "batch_quads.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../renderer.h"
#include "../vertex_buffer.h"
#include "../vertex_buffer_layout.h"

static float positions[] = {
    0.0f, 0.0f,
    100.0f, 0.0f,
    100.0f, 100.0f,
    0.0f, 100.0f,

    300.0f, 300.0f,
    400.0f, 300.0f,
    400.0f, 400.0f,
    300.0f, 400.0f
};

static unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4
};

void batch_quads_scene_init(struct BatchQuadsScene *scene) {
    vao_init(&scene->vao);

    struct VertexBuffer vbo;
    vbo_init(&vbo, positions, sizeof(positions));

    ibo_init(&scene->ibo, indices, sizeof(indices) / sizeof(indices[0]));

    struct Layout layout;
    layout_init(&layout);
    layout_push_f32(&layout, 2);

    vao_add_buffer(&scene->vao, vbo, layout);

    shader_init(&scene->shader, "res/shaders/basic.vertex.shader", "res/shaders/basic.fragment.shader");

    shader_set_uniform_4f(&scene->shader, "u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

    scene->name = "batch quads scene";
}

void batch_quads_scene_update(struct BatchQuadsScene *self) {
    // nothing
}

void batch_quads_scene_draw(struct BatchQuadsScene *self) {
    renderer_clear();

    glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(50, 50, 0));

    glm::mat4 mvp = projection * view * model;
    shader_set_uniform_mat4f(&self->shader, "u_MVP", mvp);

    renderer_draw(&self->shader, &self->vao, &self->ibo);
}

void batch_quads_scene_imgui(struct BatchQuadsScene *self) {
    // no imgui controls for now
}
