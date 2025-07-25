#include "scene_tama.hpp"

#include "raylib.h"
#include "game.hpp"

void SceneTama::OnInitialize(){
    m_game->m_transitionManager.FadeIn();
    m_game->m_audioManager->PlayTrack("music/tamagotchi.ogg");

    m_tamaui.OnInitialize();
    m_tamapetai.OnInitialize();
}

void SceneTama::OnTerminate(){
    m_tamaui.OnTerminate();
}

void SceneTama::OnUpdate(float deltaTime){
    m_tamaui.OnUpdate(deltaTime);
    m_tamapetai.OnUpdate(deltaTime);

    if (m_game->m_inputController.IsButtonBack) {
        m_game->m_sceneManager.OnChangeScene("splash");
        return;
    }
}

bool SceneTama::OnHandleInput(Vector2 mousePos) {
    bool petAIResult = m_tamapetai.OnHandleInput(mousePos);
    if (petAIResult) {
        return true;
    }

    return false;
}

void SceneTama::OnRender(){
    Texture& wallpaperTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentWallpaper());

    DrawTexture(wallpaperTexture, 0, 0, WHITE);

    m_tamapetai.OnRender();

    DrawRectangle(0, 0, 640, 80, LIGHTGRAY);
    DrawRectangle(0, 480 - 80, 640, 80, LIGHTGRAY);

    m_tamaui.OnRender();
}

void SceneTama::OnRenderUI(){

}
