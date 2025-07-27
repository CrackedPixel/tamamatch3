#include "scene_menu.hpp"

#include "game.hpp"

void SceneMenu::OnInitialize() {
    m_game->m_transitionManager.FadeIn(0.75f);
}

void SceneMenu::OnUpdate(float deltaTime) {
    if (m_game->IsChangingScenes()) {
        return;
    }

    if (m_game->m_inputController.IsButtonUp) {
        m_selectionId -= 1;

        if (m_selectionId < 0) {
            m_selectionId = m_menuList.size() - 1;
        }

        return;
    }

    if (m_game->m_inputController.IsButtonDown) {
        m_selectionId += 1;

        if (m_selectionId >= static_cast<int>(m_menuList.size())) {
            m_selectionId = 0;
        }

        return;
    }

    if (m_game->m_inputController.IsButtonSelect) {
        switch (m_selectionId) {
            case 0: { // new game
                m_game->m_gameData.NewGame();
                m_game->ChangeScene("tama", true, 1.0f);
            } break;
            case 1: { // load

            } break;
            case 2: { // credits
                m_game->ChangeScene("credits", true, 1.0f);
            } break;
            case 3: { // quit

            } break;
        }

        return;
    }
}

void SceneMenu::OnRender() {
    // TODO: render background
    // TODO: render menu
    Color drawColour;
    for (size_t i = 0; i < m_menuList.size(); ++i) {
        drawColour = m_selectionId == static_cast<int>(i) ? RED : BLACK;
        rlDrawText(m_menuList[i].c_str(), 320 - (MeasureText(m_menuList[i].c_str(), FONT_SIZE) * 0.5f), START_Y + (SPACE_Y * i), FONT_SIZE, drawColour);
    }
}

bool SceneMenu::OnHandleInput(Vector2 mousePos) {
    return false;
}
