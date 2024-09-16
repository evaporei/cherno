#include "scene.h"

#include "imgui/imgui.h"

void scene_init(struct Scene *scene, enum SceneType typ) {
    scene->typ = typ;
    textures_2D_scene_init(&scene->textures2D);
    square_scene_init(&scene->square);
    batch_quads_scene_init(&scene->batchQuads);
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
        case BATCH_QUADS_SCENE:
            batch_quads_scene_update(&scene->batchQuads);
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
        case BATCH_QUADS_SCENE:
            batch_quads_scene_draw(&scene->batchQuads);
            break;
    }
}

void scene_imgui(struct Scene *scene) {
    switch (scene->typ) {
        case TEXTURES_2D_SCENE:
            ImGui::Begin(scene->textures2D.name);

            if (ImGui::Button(scene->square.name))
                scene_choose(scene, SQUARE_SCENE);
            else if (ImGui::Button(scene->batchQuads.name))
                scene_choose(scene, BATCH_QUADS_SCENE);

            textures_2D_scene_imgui(&scene->textures2D);
            break;
        case SQUARE_SCENE:
            ImGui::Begin(scene->square.name);

            if (ImGui::Button(scene->textures2D.name))
                scene_choose(scene, TEXTURES_2D_SCENE);
            else if (ImGui::Button(scene->batchQuads.name))
                scene_choose(scene, BATCH_QUADS_SCENE);

            square_scene_imgui(&scene->square);
            break;
        case BATCH_QUADS_SCENE:
            ImGui::Begin(scene->batchQuads.name);

            if (ImGui::Button(scene->textures2D.name))
                scene_choose(scene, TEXTURES_2D_SCENE);
            else if (ImGui::Button(scene->square.name))
                scene_choose(scene, SQUARE_SCENE);

            batch_quads_scene_imgui(&scene->batchQuads);
            break;
    }
}
