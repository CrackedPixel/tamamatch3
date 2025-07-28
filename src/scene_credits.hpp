#pragma once

#include "scene.hpp"

struct SceneCredits : public Scene {
    SceneCredits(Game* game) : Scene(game) {}

    void OnInitialize();
    void OnUpdate(float deltaTime);
    void OnRender();
    bool OnHandleInput(Vector2 mousePos);

private:
    static constexpr const float SCROLL_SPEED = 50.0f;
    static constexpr const float MAX_SCROLL = 1317.0f;
    float m_offsetY = 0.0f;
};
