#pragma once

#include "../index_buffer.h"
#include "../shader.h"
#include "../vertex_array.h"

struct SquareScene {
    struct VertexArray vao;
    struct IndexBuffer ibo;
    struct Shader shader;

    float red;
    float increment;
};

void square_scene_init(struct SquareScene *self);

void square_scene_update(struct SquareScene *self);

void square_scene_draw(struct SquareScene *self);

void square_scene_imgui(struct SquareScene *self);
