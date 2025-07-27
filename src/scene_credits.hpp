#pragma once

#include "scene.hpp"

struct SceneCredits : public Scene {
    SceneCredits(Game* game) : Scene(game) {}

    void OnInitialize();
    void OnUpdate(float deltaTime);
    void OnRender();
    bool OnHandleInput(Vector2 mousePos);

private:

};
