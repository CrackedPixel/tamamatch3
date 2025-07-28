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
    if (m_game->m_inputController.IsButtonBack) {
        if (m_tamaui.IsUIShown() && m_tamaui.m_popupMenu == POPUP_TYPES::NONE) {
            m_game->m_sceneManager.OnChangeScene("splash");
            return;
        }
    }

    m_tamaui.OnUpdate(deltaTime);
    m_tamapetai.OnUpdate(deltaTime);
}

bool SceneTama::OnHandleInput(Vector2 mousePos) {
    bool uiResult = m_tamaui.OnHandleInput(m_tamapetai.GetPetPosition());
    if (uiResult) {
        return true;
    }

    bool petAIResult = m_tamapetai.OnHandleInput(mousePos);
    if (petAIResult) {
        return true;
    }

    return false;
}

void SceneTama::OnRender(){
    Texture& wallpaperTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentWallpaper());

    DrawTexture(wallpaperTexture, 0, 0, WHITE);

    if (m_game->m_gameData.GetCurrentPet().attributes[PET_ATTRIBUTES::TANKHYGIENE] < 0.5f) {
        Texture& dirtyTexture = m_game->m_resourceManager.GetTexture("textures/dirty.png");
        DrawTexture(dirtyTexture, 0, 0, ColorAlpha(WHITE, (0.5f - m_game->m_gameData.GetCurrentPet().attributes[PET_ATTRIBUTES::TANKHYGIENE]) * 2));
    }

    m_tamapetai.OnRender();
}

void SceneTama::OnRenderUI(){
    Texture& cursorTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentCursorPath());//"textures/cursors.png", 0);

    m_tamapetai.OnRenderUI();
    m_tamaui.OnRenderUI();

    Vector2 mousePosition = GetMousePosition();
    mousePosition.x -= 184;
    mousePosition.y -= 174;

    if (CheckCollisionPointRec(mousePosition, { 0, 0, 640, 480 })) {
#ifdef PLATFORM_DESKTOP
        rlHideCursor();
#endif

        DrawTexturePro(
            cursorTexture,
            m_game->m_gameData.GetCurrentCursorCoords(),
            { mousePosition.x, mousePosition.y, 32, 32 },
            { 0, 0 },
            0.0f,
            WHITE
        );
    } else {
#ifdef PLATFORM_DESKTOP
        rlShowCursor();
#endif
    }
}
