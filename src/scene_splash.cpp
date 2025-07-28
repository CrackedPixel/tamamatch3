#include "scene_splash.hpp"

#include "raylib.h"
#include "game.hpp"

void SceneSplash::OnInitialize() {
    m_startedFade = false;
    m_currentTimer = 0.0f;
    m_game->m_transitionManager.FadeIn(0.0f);
}

void SceneSplash::OnUpdate(float deltaTime){
    if (m_game->IsChangingScenes()) {
        return;
    }

    m_currentTimer += deltaTime;

    if (!m_startedFade && m_currentTimer >= 1.0f) {
        m_startedFade = true;
        m_game->m_transitionManager.FadeIn(0.5f);
    }

    if (m_currentTimer >= 7.0f) {
        m_game->ChangeScene("menu", true, 0.5f);
    }

    if (m_game->m_inputController.IsButtonSelect) {
        m_game->ChangeScene("menu", true, 0.75f);
    }
}

void SceneSplash::OnRender(){
    DrawRectangle(0, 0, 640, 480, { 180, 180, 180, 255 });

    Texture& logoTexture = m_game->m_resourceManager.GetTexture("textures/notmarmalade.png", 0);

    DrawTexturePro(logoTexture, { 0, 0, 46, 51 }, { 274, 189, 92, 102 }, { 0, 0 }, 0.0f, WHITE);
    rlDrawText("2025 gamejam", 320 - (MeasureText("2025 gamejam", 20) * 0.5f), 300, 20, BLACK);
}

void SceneSplash::OnRenderUI() {
    Texture& cursorTexture = m_game->m_resourceManager.GetTexture("textures/cursors.png");

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
