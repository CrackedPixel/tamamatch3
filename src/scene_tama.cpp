#include "scene_tama.hpp"

#include "raylib.h"
#include "game.hpp"

void SceneTama::OnInitialize(){
    printf("initialize tama\n");
    fflush(stdout);
    m_game->m_transitionManager.FadeIn();
    m_game->m_audioManager->PlayTrack("music/tamagotchi.ogg");
}

void SceneTama::OnTerminate(){

}

void SceneTama::OnUpdate(float deltaTime){

}

void SceneTama::OnRender(){
    Texture& deviceTexture = m_game->m_resourceManager.GetTexture("textures/castle.png");

    DrawTexture(deviceTexture, 0, 0, WHITE);
}

void SceneTama::OnRenderUI(){

}

void SceneTama::OnHandleInput(Vector2 mousePos) {

}
