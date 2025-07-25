#pragma once

#include "scene.hpp"

struct SceneSplash : public Scene {
    SceneSplash(Game* m_game) : Scene(m_game) {}

    void OnInitialize();
    void OnTerminate();
    void OnUpdate(float deltaTime);
    void OnRender();
    void OnRenderUI();
    void OnHandleInput(Vector2 mousePos);
};
