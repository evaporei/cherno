#pragma once

#include "../index_buffer.h"
#include "../shader.h"
#include "../vertex_array.h"

struct BatchQuadsScene {
    const char *name;

    struct VertexArray vao;
    struct IndexBuffer ibo;
    struct Shader shader;
};

void batch_quads_scene_init(struct BatchQuadsScene *self);

void batch_quads_scene_update(struct BatchQuadsScene *self);

void batch_quads_scene_draw(struct BatchQuadsScene *self);

void batch_quads_scene_imgui(struct BatchQuadsScene *self);
