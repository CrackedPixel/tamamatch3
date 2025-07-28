#pragma once

#include "scene.hpp"

struct SceneCollection : public Scene {
    SceneCollection(Game* game) : Scene(game) {}

    void OnInitialize();
    void OnUpdate(float deltaTime);
    void OnRender();

private:
    void DrawPetById(int slot, int petId);

private:
    int m_selectedId = 0;
};
