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
                m_game->ChangeScene("tama", true, 0.5f);
                return;
            } break;
            case MASTERMIND_STATES::STARTED: {
                if (m_selectionId == 0) {
                    m_game->ChangeScene("tama", true, 0.5f);
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

}

void SceneMastermind::OnRenderUI() {

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
}

void SceneMastermind::SubmitGuess() {

}
