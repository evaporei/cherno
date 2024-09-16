#include "scene.h"

void scene_init(struct Scene *scene, enum SceneType typ) {
    scene->typ = typ;
    switch (scene->typ) {
        case TEXTURES_2D_SCENE:
            textures_2D_scene_init(&scene->textures2D);
            break;
        case SQUARE_SCENE:
            square_scene_init(&scene->square);
            break;
    }
}

void scene_choose(struct Scene *scene, enum SceneType typ) {
    scene->typ = typ;
}

void scene_update(struct Scene *scene) {
    switch (scene->typ) {
        case TEXTURES_2D_SCENE:
            textures_2D_scene_update(&scene->textures2D);
            break;
        case SQUARE_SCENE:
            square_scene_update(&scene->square);
            break;
    }
}

void scene_draw(struct Scene *scene) {
    switch (scene->typ) {
        case TEXTURES_2D_SCENE:
            textures_2D_scene_draw(&scene->textures2D);
            break;
        case SQUARE_SCENE:
            square_scene_draw(&scene->square);
            break;
    }
}

void scene_imgui(struct Scene *scene) {
    switch (scene->typ) {
        case TEXTURES_2D_SCENE:
            textures_2D_scene_imgui(&scene->textures2D);
            break;
        case SQUARE_SCENE:
            square_scene_imgui(&scene->square);
            break;
    }
}
