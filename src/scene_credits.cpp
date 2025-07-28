#include "scene_credits.hpp"

#include "game.hpp"

void SceneCredits::OnInitialize() {
    m_offsetY = 0.0f;
    m_game->m_transitionManager.FadeIn(0.75f);
}

void SceneCredits::OnUpdate(float deltaTime) {
    if (m_game->IsChangingScenes()) {
        return;
    }

    m_offsetY += deltaTime * SCROLL_SPEED;

    if (m_game->m_inputController.IsButtonDown) {
        m_offsetY += deltaTime * SCROLL_SPEED * 50;
    }

    if (m_offsetY >= MAX_SCROLL) {
        m_offsetY = MAX_SCROLL;
    }

    if (m_game->m_inputController.IsButtonBack) {
        m_game->ChangeScene("menu", true, 1.0f);
    }

    if (m_game->m_inputController.IsButtonSelect) {
        m_game->ChangeScene("menu", true, 1.0f);
    }
}

void SceneCredits::OnRender() {
    Texture& wallpaperTexture = m_game->m_resourceManager.GetTexture("textures/wall1.png");
    Texture& creditsTexture = m_game->m_resourceManager.GetTexture("textures/credits.png");
    Texture& cursorTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentCursorPath(), 0);

    DrawTexture(wallpaperTexture, 0, 0, WHITE);
    DrawRectangle(0, 0, 640, 480, ColorAlpha(GREEN, 0.25f));
    DrawTexture(creditsTexture, 0, 0 - m_offsetY, WHITE);


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

bool SceneCredits::OnHandleInput(Vector2 mousePos) {
    return false;
}
