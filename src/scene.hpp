#pragma once

#include "raylib.h"

struct Game;

struct Scene {
    Scene(Game* game) : m_game(game) {}
    virtual ~Scene() = default;
    virtual void OnInitialize() = 0;
    virtual void OnTerminate() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnRender() = 0;
    virtual void OnRenderUI() = 0;
    virtual void OnHandleInput(Vector2 mousePos) = 0;

protected:
    Game* m_game;
};
