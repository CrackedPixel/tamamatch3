#pragma once

#include "scene.hpp"

struct SceneTama : public Scene {
    SceneTama(Game* game) : Scene(game) {}

    void OnInitialize();
    void OnTerminate();
    void OnUpdate(float deltaTime);
    void OnRender();
    void OnRenderUI();
    void OnHandleInput(Vector2 mousePos);
};
