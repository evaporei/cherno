#include "scene.h"

void scene_init(struct Scene *scene) {
    scene->typ = TEXTURES_2D_SCENE;
    switch (scene->typ) {
        case TEXTURES_2D_SCENE:
            textures_2D_scene_init(&scene->textures2D);
    }
}

void scene_update(struct Scene *scene) {
    switch (scene->typ) {
        case TEXTURES_2D_SCENE:
            textures_2D_scene_update(&scene->textures2D);
    }
}

void scene_draw(struct Scene *scene) {
    switch (scene->typ) {
        case TEXTURES_2D_SCENE:
            textures_2D_scene_draw(&scene->textures2D);
    }
}

void scene_imgui(struct Scene *scene) {
    switch (scene->typ) {
        case TEXTURES_2D_SCENE:
            textures_2D_scene_imgui(&scene->textures2D);
    }
}
