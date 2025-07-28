#include "scene_menu.hpp"

#include "game.hpp"

void SceneMenu::OnInitialize() {
    m_game->m_transitionManager.FadeIn(0.75f);
    m_game->m_audioManager->PlayTrack("music/menu.ogg");
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
                m_game->Quit();
            } break;
        }

        return;
    }
}

void SceneMenu::OnRender() {
    // TODO: render background
    Texture& logoTexture = m_game->m_resourceManager.GetTexture("textures/logo.png");
    Texture& wallpaperTexture = m_game->m_resourceManager.GetTexture("textures/wall1.png");
    Texture& frogeTexture = m_game->m_resourceManager.GetTexture("textures/frogeanimation.png");

    int animationIndex = static_cast<int>(GetTime() * 2.0f) % 16;

    DrawTexture(wallpaperTexture, 0, 0, WHITE);
    DrawTexture(logoTexture, 0, 0, WHITE);
    DrawTexturePro(frogeTexture, { (animationIndex % 4) * 128.0f, (animationIndex / 4) * 128.0f, 128, 128 }, { 500, 350, 128, 128 }, { 0, 0 }, 0.0f, WHITE);

    Color highlightColour;
    Color fontColour;
    for (size_t i = 0; i < m_menuList.size(); ++i) {
        highlightColour = m_selectionId == static_cast<int>(i) ? Color{ 175, 246, 132, 255 } : Color{ 191, 240, 161, 255 };
        fontColour = m_selectionId == static_cast<int>(i) ? Color{ 45, 170, 117, 255 } : BLACK;
        DrawRectangleRounded({ 224, START_Y + (SPACE_Y * i) - (38 * 0.25f), 188, 38 }, 0.4f, 0, highlightColour);
        DrawRectangleRoundedLinesEx({ 224, START_Y + (SPACE_Y * i) - (38 * 0.25f), 188, 38 }, 0.4f, 0, 2, BLACK);
        rlDrawText(m_menuList[i].c_str(), 320 - (MeasureText(m_menuList[i].c_str(), FONT_SIZE) * 0.5f), START_Y + (SPACE_Y * i), FONT_SIZE, fontColour);
    }
}
