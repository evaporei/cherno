#pragma once

#include "textures_2D.h"

enum SceneType {
    TEXTURES_2D_SCENE = 0
};

struct Scene {
    enum SceneType typ;
    struct Textures2DScene textures2D;
};

void scene_init(struct Scene *scene);

void scene_update(struct Scene *scene);

void scene_draw(struct Scene *scene);

void scene_imgui(struct Scene *scene);
