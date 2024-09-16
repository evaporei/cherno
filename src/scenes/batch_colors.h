#pragma once

#include "../index_buffer.h"
#include "../shader.h"
#include "../vertex_array.h"

struct BatchColorsScene {
    const char *name;

    struct VertexArray vao;
    struct IndexBuffer ibo;
    struct Shader shader;
};

void batch_colors_scene_init(struct BatchColorsScene *self);

void batch_colors_scene_update(struct BatchColorsScene *self);

void batch_colors_scene_draw(struct BatchColorsScene *self);

void batch_colors_scene_imgui(struct BatchColorsScene *self);
