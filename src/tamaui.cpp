#include "tamaui.hpp"

#include "game.hpp"

void TamaUI::OnInitialize() {
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::SETTINGS, { 0, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::MINIGAMES, { 64, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::MINIGAMES, { 64, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::SETTINGS, { 0, 0, 64, 64 } });

    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::MINIGAMES, { 64, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::MINIGAMES, { 64, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::SETTINGS, { 0, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::MINIGAMES, { 64, 0, 64, 64 } });
}

void TamaUI::OnTerminate() {
    m_icons.clear();
}

void TamaUI::OnUpdate(float deltaTime) {
    if (m_game->m_inputController.IsButtonUp) {
        m_hoverId -= 1;

        if (m_hoverId < 0) {
            m_hoverId = m_icons.size() - 1;
        }
    }

    if (m_game->m_inputController.IsButtonDown) {
        m_hoverId += 1;

        if (m_hoverId == static_cast<int>(m_icons.size())) {
            m_hoverId = 0;
        }
    }
}

void TamaUI::OnHandleInput() {

}

void TamaUI::OnRender() {
    Texture& iconsTexture = m_game->m_resourceManager.GetTexture(TEXTURE_PATH);

    Color drawColour;
    rlRectangle destination = { 0.0f, 0.0f, 64, 64 };
    for (size_t i = 0; i < m_icons.size(); ++i) {
        drawColour = m_hoverId == static_cast<int>(i) ? WHITE : BLACK;
        if (i < 4) {
            destination.x = 80.0f  + ( 130.0f * i);
            destination.y = 8;

            if (m_hoverId == static_cast<int>(i)) {
                DrawRectangle(destination.x - 8, destination.y - 8, destination.width + 16, destination.height + 16, RED);
            }

            DrawTexturePro(iconsTexture, m_icons[i].sourceRect, destination, { 0.0f, 0.0f }, 0.0f, drawColour);
        } else if (i < 8) {
            destination.x = 80.0f  + ( 130.0f * (i - 4));
            destination.y = 480 - 80 + 8;

            if (m_hoverId == static_cast<int>(i)) {
                DrawRectangle(destination.x - 8, destination.y - 8, destination.width + 16, destination.height + 16, RED);
            }

            DrawTexturePro(iconsTexture, m_icons[i].sourceRect, destination, { 0.0f, 0.0f }, 0.0f, drawColour);
        } else {
            // nothing?
        }
    }
}
