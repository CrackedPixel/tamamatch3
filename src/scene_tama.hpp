#pragma once

#include "scene.hpp"
#include "tamaui.hpp"
#include "tamapetai.hpp"

struct SceneTama : public Scene {
    SceneTama(Game* game) : Scene(game), m_tamaui(game), m_tamapetai(game) {}

    void OnInitialize();
    void OnTerminate();
    void OnUpdate(float deltaTime);
    void OnRender();
    void OnRenderUI();
    bool OnHandleInput(Vector2 mousePos);

private:
    TamaUI m_tamaui;
    TamaPetAI m_tamapetai;
};
