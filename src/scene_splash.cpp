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
    Vector2 mousePosition = GetMousePosition();
    mousePosition.x -= 184;
    mousePosition.y -= 174;

    if (CheckCollisionPointRec(mousePosition, { 0, 0, 640, 480 })) {
#ifdef PLATFORM_DESKTOP
        rlHideCursor();
#endif
        Texture& cursorTexture = m_game->m_resourceManager.GetTexture("textures/cursors.png");

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

void SceneSplash::OnRenderUI(){

}

bool SceneSplash::OnHandleInput(Vector2 mousePos) {
    mousePos.x -= 184;
    mousePos.y -= 174;

    if (CheckCollisionPointRec(mousePos, { 0, 0, 640, 480 })) {
#ifdef PLATFORM_DESKTOP
        rlHideCursor();
#endif
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_game->m_inputController.EnableButtonSelect();
            return true;
        }
    } else {
#ifdef PLATFORM_DESKTOP
        rlShowCursor();
#endif
    }

    return false;
}
