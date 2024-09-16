#include "square.h"

#include "../renderer.h"
#include "../vertex_buffer.h"
#include "../vertex_buffer_layout.h"

static float positions[] = {
    -50.0f, -50.0f,
    50.0f, -50.0f,
    50.0f, 50.0f,
    -50.0f, 50.0f
};

static unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

void square_scene_init(struct SquareScene *scene) {
    struct VertexArray vao = vao_init();

    struct VertexBuffer vbo = vbo_init(positions, sizeof(positions));

    struct IndexBuffer ibo = ibo_init(indices, sizeof(indices) / sizeof(indices[0]));

    struct Layout layout = layout_init();
    layout_push_f32(&layout, 2);

    vao_add_buffer(&vao, vbo, layout);

    struct Shader shader = shader_init("res/shaders/basic.vertex.shader", "res/shaders/basic.fragment.shader");

    shader_set_uniform_4f(&shader, "u_Color", 0.8, 0.3, 0.8, 1.0);

    scene->vao = vao;
    scene->ibo = ibo;
    scene->shader = shader;
}

void square_scene_update(struct SquareScene *self) {
    // no update for now
}

void square_scene_draw(struct SquareScene *self) {
    shader_set_uniform_4f(&self->shader, "u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    renderer_draw(&self->shader, &self->vao, &self->ibo);
}

void square_scene_imgui(struct SquareScene *self) {
    // no imgui controls for now
}
