#include "scene_mastermind.hpp"

#include "game.hpp"

void SceneMastermind::OnInitialize() {
#ifdef DEBUG_BUILD
    m_game->m_transitionManager.FadeIn(20.0f);
#else
    m_game->m_transitionManager.FadeIn(0.5f);
#endif
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

                m_previousGuesses[m_currentGuess].value[m_selectionId] = -1;
                m_selectionId -= 1;
                return;
            } break;
            case MASTERMIND_STATES::WIN: {
                m_game->ChangeScene("tama", true, 1.0f);
                return;
            } break;
            case MASTERMIND_STATES::LOSE: {
                m_game->ChangeScene("tama", true, 1.0f);
                return;
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
            if (m_previousGuesses[m_currentGuess].value[m_selectionId] < 0) {
                m_previousGuesses[m_currentGuess].value[m_selectionId] = 0;
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

    if (m_game->m_inputController.IsButtonUp) {
        switch (m_state) {
            case MASTERMIND_STATES::STARTED: {
                m_previousGuesses[m_currentGuess].value[m_selectionId] -= 1;

                if (m_previousGuesses[m_currentGuess].value[m_selectionId] == -1) {
                    m_previousGuesses[m_currentGuess].value[m_selectionId] = 4;
                }
                return;
            } break;
            default: break;
        }
    }

    if (m_game->m_inputController.IsButtonDown) {
        switch (m_state) {
        case MASTERMIND_STATES::STARTED: {
            m_previousGuesses[m_currentGuess].value[m_selectionId] += 1;

            if (m_previousGuesses[m_currentGuess].value[m_selectionId] == 5) {
                m_previousGuesses[m_currentGuess].value[m_selectionId] = 0;
            }
            return;
        } break;
        default: break;
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

            rlDrawText(TextFormat("%d", m_currentGuess+1), 320 - (140.0f + 20) - 10 - (MeasureText(TextFormat("%d", m_currentGuess+1), 30)), 400.0f, 30, BLACK);

            for (int i = 0; i < 4; ++i) {
                DrawTexturePro(
                    mastermindTexture,
                    m_angledPadTexturePos,
                    { 320 - (140.0f + 20) + (i * (m_angledPadTexturePos.width + 10)), 400.0f, m_angledPadTexturePos.width, m_angledPadTexturePos.height },
                    { 0, 0 },
                    0.0f,
                    WHITE
                );

                if (m_previousGuesses[m_currentGuess].value[i] >= 0) {
                    DrawTexturePro(
                        mastermindTexture,
                        m_eggTexturePos,
                        { 320 - (140.0f + 20) + (i * (m_angledPadTexturePos.width + 10)) + 20, 400.0f - 10, m_eggTexturePos.width, m_eggTexturePos.height },
                        { 0, 0 },
                        0.0f,
                        m_guessColours[m_previousGuesses[m_currentGuess].value[i]]
                    );
                }
            }

            int displayed = 0;
            float offsetY = 0.0f;
            float offsetX = m_angledPadTexturePos.width + 10;
            for (int guessId = m_currentGuess - GUESS_HISTORY; guessId < m_currentGuess; ++guessId) {
                if (guessId < 0) {
                    continue;
                }
                offsetY = displayed * 50.0f;
                rlDrawText(TextFormat("%d", guessId+1), 320 - (140.0f + 20) - 10 - (MeasureText(TextFormat("%d", guessId+1), 30)) - offsetX, 50.0f + offsetY, 30, BLACK);
                for (int i = 0; i < 4; ++i) {
                    DrawTexturePro(
                        mastermindTexture,
                        m_angledPadTexturePos,
                        { 320 - (140.0f + 20) + (i * (m_angledPadTexturePos.width + 10)) - offsetX, 50.0f + offsetY, m_angledPadTexturePos.width, m_angledPadTexturePos.height },
                        { 0, 0 },
                        0.0f,
                        WHITE
                    );

                    if (m_previousGuesses[guessId].value[i] >= 0) {
                        DrawTexturePro(
                            mastermindTexture,
                            m_eggTexturePos,
                            { 320 - (140.0f + 20) + (i * (m_angledPadTexturePos.width + 10)) + 20 - offsetX, 50.0f - 10 + offsetY, m_eggTexturePos.width, m_eggTexturePos.height },
                            { 0, 0 },
                            0.0f,
                            m_guessColours[m_previousGuesses[guessId].value[i]]
                        );
                    }
                }

                for (int i = 0; i < (m_previousResults[guessId].rightColour + m_previousResults[guessId].rightColourAndSpot); ++i) {
                    DrawTexturePro(
                        mastermindTexture,
                        m_lilyTexturePos,
                        { 320 - (140.0f + 20) + (5 * (m_angledPadTexturePos.width + 10)) + 20 - offsetX, 50.0f - 10 + offsetY, m_lilyTexturePos.width, m_lilyTexturePos.height },
                        { 0, 0 },
                        0.0f,
                        i < m_previousResults[guessId].rightColourAndSpot ? WHITE : RED
                    );
                }

                displayed += 1;
            }
            return;
        } break;
        case MASTERMIND_STATES::WIN: {
            rlDrawText("WIN", 20, 20, 50, RED);
            return;
        } break;
        case MASTERMIND_STATES::LOSE: {
            rlDrawText("LOSE", 20, 20, 50, RED);
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
        m_previousGuesses[i].value[0] = -1;
        m_previousGuesses[i].value[1] = -1;
        m_previousGuesses[i].value[2] = -1;
        m_previousGuesses[i].value[3] = -1;
    }

    m_currentGuess = 0;
    m_selectionId = 0;

    m_correctAnswer.value[0] = GetRandomValue(0, 4);
    m_correctAnswer.value[1] = GetRandomValue(0, 4);
    m_correctAnswer.value[2] = GetRandomValue(0, 4);
    m_correctAnswer.value[3] = GetRandomValue(0, 4);

    m_previousGuesses[0].value[0] = 0;
}

void SceneMastermind::SubmitGuess() {
    bool usedInAnswer[4] = { false, false, false, false };
    bool usedInGuess[4] = { false, false, false, false };

    m_previousResults[m_currentGuess].rightColourAndSpot = 0;
    m_previousResults[m_currentGuess].rightColour = 0;

    for (int i = 0; i < 4; ++i) {
        if (m_previousGuesses[m_currentGuess].value[i] == m_correctAnswer.value[i]) {
            m_previousResults[m_currentGuess].rightColourAndSpot++;
            usedInAnswer[i] = true;
            usedInGuess[i] = true;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (!usedInGuess[i]) {
            for (int x = 0; x < 4; ++x) {
                if (!usedInAnswer[x] && m_previousGuesses[m_currentGuess].value[i] == m_correctAnswer.value[x]) {
                    m_previousResults[m_currentGuess].rightColour++;
                    usedInAnswer[x] = true;
                    break;
                }
            }
        }
    }

    if (m_previousResults[m_currentGuess].rightColourAndSpot == 4) {
        m_state = MASTERMIND_STATES::WIN;

        return;
    }

    m_currentGuess += 1;
    m_selectionId = 0;

    if (m_currentGuess == MAX_GUESS) {
        m_state = MASTERMIND_STATES::LOSE;

        return;
    }

    m_previousGuesses[m_currentGuess].value[0] = 0;
}
