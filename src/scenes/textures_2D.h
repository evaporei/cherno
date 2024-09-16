#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../index_buffer.h"
#include "../shader.h"
#include "../vertex_array.h"

struct Textures2DScene {
    const char *name;

    struct VertexArray vao;
    struct IndexBuffer ibo;
    struct Shader shader;

    glm::vec3 translationA;
    glm::vec3 translationB;
};

void textures_2D_scene_init(struct Textures2DScene *self);

void textures_2D_scene_update(struct Textures2DScene *self);

void textures_2D_scene_draw(struct Textures2DScene *self);

void textures_2D_scene_imgui(struct Textures2DScene *self);
