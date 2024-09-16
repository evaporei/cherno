#pragma once

#include "textures_2D.h"
#include "square.h"
#include "batch_quads.h"

enum SceneType {
    TEXTURES_2D_SCENE = 0,
    SQUARE_SCENE = 1,
    BATCH_QUADS_SCENE = 2
};

struct Scene {
    enum SceneType typ;
    struct Textures2DScene textures2D;
    struct SquareScene square;
    struct BatchQuadsScene batchQuads;
};

void scene_init(struct Scene *scene, enum SceneType typ);

void scene_choose(struct Scene *scene, enum SceneType typ);

void scene_update(struct Scene *scene);

void scene_draw(struct Scene *scene);

void scene_imgui(struct Scene *scene);
