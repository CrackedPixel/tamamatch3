#pragma once

#include "scene.hpp"
#include "tamaui.hpp"
#include "tamapetai.hpp"

struct SceneTama : public Scene {
    SceneTama(Game* game) : Scene(game), m_tamaui(game, this), m_tamapetai(game, this) {}

    void OnInitialize();
    void OnTerminate();
    void OnUpdate(float deltaTime);
    void OnRender();
    void OnRenderUI();
    bool OnHandleInput(Vector2 mousePos);

public:
    TamaPetAI* GetPetAI() {
        return &m_tamapetai;
    }

    TamaUI* GetTamaUI() {
        return & m_tamaui;
    }

public:


private:
    TamaUI m_tamaui;
    TamaPetAI m_tamapetai;
};
