#include "scene_splash.hpp"

#include "raylib.h"
#include "game.hpp"

void SceneSplash::OnInitialize(){
    m_game->m_transitionManager.FadeIn();
    m_game->m_audioManager->PlayTrack("music/menu.ogg");
}

void SceneSplash::OnTerminate(){

}

void SceneSplash::OnUpdate(float deltaTime){
    if (m_game->m_inputController.IsButtonSelect) {
        m_game->ChangeScene("tama", true);
    }
}

void SceneSplash::OnRender(){
    rlDrawText("SPLASH", 0, 0, 50, BLUE);
}

void SceneSplash::OnRenderUI(){

}

bool SceneSplash::OnHandleInput(Vector2 mousePos) {
    return false;
}
