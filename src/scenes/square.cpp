#include "square.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../renderer.h"
#include "../vertex_buffer.h"
#include "../vertex_buffer_layout.h"

static float positions[] = {
    -200.0f, -200.0f,
    200.0f, -200.0f,
    200.0f, 200.0f,
    -200.0f, 200.0f
};

static unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

void square_scene_init(struct SquareScene *scene) {
    vao_init(&scene->vao);

    struct VertexBuffer vbo;
    vbo_init(&vbo, positions, sizeof(positions));

    ibo_init(&scene->ibo, indices, sizeof(indices) / sizeof(indices[0]));

    struct Layout layout;
    layout_init(&layout);
    layout_push_f32(&layout, 2);

    vao_add_buffer(&scene->vao, vbo, layout);

    shader_init(&scene->shader, "res/shaders/basic.vertex.shader", "res/shaders/basic.fragment.shader");

    shader_set_uniform_4f(&scene->shader, "u_Color", 0.8, 0.3, 0.8, 1.0);

    scene->name = "square scene";
    scene->red = 0;
    scene->increment = 0.008;
}

void square_scene_update(struct SquareScene *self) {
    if (self->red > 1.0)
        self->increment = -0.008;
    else if (self->red < 0.0)
        self->increment = 0.008;

    self->red += self->increment;
}

void square_scene_draw(struct SquareScene *self) {
    renderer_clear();

    glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(480, 270, 0));

    glm::mat4 mvp = projection * view * model;

    shader_bind(&self->shader);
    shader_set_uniform_mat4f(&self->shader, "u_MVP", mvp);
    shader_set_uniform_4f(&self->shader, "u_Color", self->red, 0.3f, 0.8f, 1.0f);

    renderer_draw(&self->vao, &self->ibo);
}

void square_scene_imgui(struct SquareScene *self) {
    // no imgui controls for now
}
