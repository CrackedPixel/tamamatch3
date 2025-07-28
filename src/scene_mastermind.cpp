#include "scene_mastermind.hpp"

#include "game.hpp"

void SceneMastermind::OnInitialize() {
    m_game->m_transitionManager.FadeIn(0.5f);
    m_state = MASTERMIND_STATES::NEW;
}

void SceneMastermind::OnUpdate(float deltaTime) {
    if (m_game->IsChangingScenes()) {
        return;
    }

    if (m_game->m_inputController.IsButtonBack) {
        switch (m_state) {
            case MASTERMIND_STATES::NEW: {
                m_game->ChangeScene("tama", true, 1.0f);
                return;
            } break;
            case MASTERMIND_STATES::STARTED: {
                if (m_selectionId == 0) {
                    m_game->ChangeScene("tama", true, 1.0f);
                    return;
                }

                m_selectionId -= 1;
                return;
            } break;
            case MASTERMIND_STATES::WIN: {

            } break;
            case MASTERMIND_STATES::LOSE: {

            } break;
        }
    }

    if (m_game->m_inputController.IsButtonSelect) {
        switch (m_state) {
        case MASTERMIND_STATES::NEW: {
            ResetBoard();
            m_state = MASTERMIND_STATES::STARTED;
            m_game->m_audioManager->PlaySFX("uiselect");
            return;
        } break;
        case MASTERMIND_STATES::STARTED: {
            if (m_selectionId == 3) {
                SubmitGuess();
                return;
            }

            m_selectionId += 1;
            return;
        } break;
        case MASTERMIND_STATES::WIN: {
            m_state = MASTERMIND_STATES::NEW;
            return;
        } break;
        case MASTERMIND_STATES::LOSE: {
            m_state = MASTERMIND_STATES::NEW;
            return;
        } break;
        }
    }
}

void SceneMastermind::OnRender() {
    Texture& wallpaperTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentWallpaper());

    DrawTexture(wallpaperTexture, 0, 0, WHITE);

    switch (m_state) {
        case MASTERMIND_STATES::NEW: {
            DrawRectangleRounded({ 320 - (380 * 0.5f), 130, 380, 230 }, 0.2f, 1, { 111, 180, 68, 255 });
            DrawRectangleRoundedLinesEx({ 320 - (380 * 0.5f), 130, 380, 230 }, 0.2f, 1, 3, BLACK);
            rlDrawText("Mastermind", 320 - (MeasureText("Mastermind", 20) * 0.5f), 150, 20, BLACK);
            rlDrawText("Press to start", 320 - (MeasureText("Press to start", 20) * 0.5f), 180, 20, BLACK);
        } break;
        case MASTERMIND_STATES::STARTED: {
            Texture& mastermindTexture = m_game->m_resourceManager.GetTexture(TEXTURE_PATH, 0);
            for (int i = 0; i < 4; ++i) {
                DrawTexturePro(mastermindTexture, m_lilyTexturePos, {  }, { 0, 0 }, 0.0f, WHITE);
            }

            for (int i = 0; i < m_currentGuess; ++i) {

            }
            return;
        } break;
        case MASTERMIND_STATES::WIN: {
            m_state = MASTERMIND_STATES::NEW;
            return;
        } break;
        case MASTERMIND_STATES::LOSE: {
            m_state = MASTERMIND_STATES::NEW;
            return;
        } break;
    }
}

void SceneMastermind::OnRenderUI() {
    Texture& cursorTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentCursorPath(), 0);

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

void SceneMastermind::ResetBoard() {
    for (int i = 0; i < MAX_GUESS; ++i) {
        m_previousGuesses[i].value[0] = 0;
        m_previousGuesses[i].value[1] = 0;
        m_previousGuesses[i].value[2] = 0;
        m_previousGuesses[i].value[3] = 0;
    }

    m_currentGuess = 0;
    m_selectionId = 0;

    m_correctAnswer.value[0] = GetRandomValue(0, 4);
    m_correctAnswer.value[1] = GetRandomValue(0, 4);
    m_correctAnswer.value[2] = GetRandomValue(0, 4);
    m_correctAnswer.value[3] = GetRandomValue(0, 4);
}

void SceneMastermind::SubmitGuess() {

}
