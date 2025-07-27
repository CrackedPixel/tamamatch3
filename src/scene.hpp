#pragma once

#include "raylib.h"

struct Game;

struct Scene {
    Scene(Game* game) : m_game(game) {}
    virtual ~Scene() = default;
    virtual void OnInitialize() {}
    virtual void OnTerminate() {}
    virtual void OnUpdate(float) {}
    virtual void OnRender() {}
    virtual void OnRenderUI() {}
    virtual bool OnHandleInput(Vector2) { return false; }

protected:
    Game* m_game;
};
