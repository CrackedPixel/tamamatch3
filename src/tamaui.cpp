#include "tamaui.hpp"

#include "game.hpp"

void TamaUI::OnInitialize() {
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::SETTINGS, { 64 * 0, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::MINIGAMES, { 64 * 1, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::BANDAID, { 64 * 2, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::TOY, { 64 * 3, 0, 64, 64 } });

    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::CAMERA, { 64 * 0, 64, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::INVENTORY, { 64 * 1, 64, 64, 64 } });
    // m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::SETTINGS, { 64 * 2, 64, 64, 64 } });
    // m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::MINIGAMES, { 64 * 3, 64, 64, 64 } });
}

void TamaUI::OnTerminate() {
    m_icons.clear();
}

void TamaUI::OnUpdate(float deltaTime) {
    if (m_game->m_inputController.IsButtonUp) {
        m_selectedId -= 1;

        if (m_selectedId < 0) {
            m_selectedId = m_icons.size() - 1;
        }

        return;
    }

    if (m_game->m_inputController.IsButtonDown) {
        m_selectedId += 1;

        if (m_selectedId == static_cast<int>(m_icons.size())) {
            m_selectedId = 0;
        }

        return;
    }

    Vector2 mousePosition = GetMousePosition();
    mousePosition.x -= 184;
    mousePosition.y -= 174;

    rlRectangle destination = { 0, 0, 80, 80 };
    for (size_t i = 0; i < m_icons.size(); ++i) {
        if (i < 4) {
            destination.x = 80.0f  + ( 130.0f * i) - 8;
            destination.y = 0;

            if (CheckCollisionPointRec(mousePosition, destination)) {
                m_hoverId = i;

                return;
            }
        } else if (i < 8) {
            destination.x = 80.0f  + ( 130.0f * (i - 4)) - 8;
            destination.y = 480 - 80;

            if (CheckCollisionPointRec(mousePosition, destination)) {
                m_hoverId = i;

                return;
            }
        }
    }

    m_hoverId = -1;
}

bool TamaUI::OnHandleInput() {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return false;
    }

    if (m_hoverId == -1) {
        return false;
    }

    // enum struct CURSOR_TYPES {
    //     NORMAL,
    //     INVALID,
    //     FOOD,
    //     SAD,
    //     BORED,
    //     DIRTY,
    //     TANKDIRTY,
    //     ILLNESS,
    //     TOY,
    // };

    switch (m_icons[m_hoverId].actionType) {
        case ICON_ACTION_TYPE::STATS: {
            // TODO: open stats
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::BANDAID: {
            m_game->m_gameData.activeCursor = CURSOR_TYPES::ILLNESS;
        } break;
        case ICON_ACTION_TYPE::CAMERA: {
            hideUI = !hideUI;

            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::CLEAN: {
            m_game->m_gameData.activeCursor = CURSOR_TYPES::DIRTY;
        } break;
        case ICON_ACTION_TYPE::CLEAN_TANK: {
            m_game->m_gameData.activeCursor = CURSOR_TYPES::TANKDIRTY;
        } break;
        case ICON_ACTION_TYPE::INVENTORY: {
            // TODO: open inventory
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::MINIGAMES: {
            // TODO: change scene
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::SETTINGS: {
            // TODO: open settings
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::TOY: {
            m_game->m_gameData.activeCursor = CURSOR_TYPES::TOY;
        } break;
    }

    return true;
}

void TamaUI::OnRenderUI() {
    if (hideUI) {
        return;
    }

    Texture& iconsTexture = m_game->m_resourceManager.GetTexture(TEXTURE_PATH);

    Color drawColour;
    Color bgColour;
    rlRectangle destination = { 0.0f, 0.0f, 64, 64 };

    for (size_t i = 0; i < m_icons.size(); ++i) {
        drawColour = WHITE; //m_selectedId == static_cast<int>(i) ? WHITE : LIGHTGRAY;
        bgColour = m_hoverId == static_cast<int>(i) ? YELLOW : RED;
        if (i < 4) {
            destination.x = 80.0f  + ( 130.0f * i);
            destination.y = 8;

            if (
                m_selectedId == static_cast<int>(i)
                || m_hoverId == static_cast<int>(i)
            ) {
                DrawRectangle(destination.x - 8, destination.y - 8, destination.width + 16, destination.height + 16, bgColour);
            }

            DrawTexturePro(iconsTexture, m_icons[i].sourceRect, destination, { 0.0f, 0.0f }, 0.0f, drawColour);
        } else if (i < 8) {
            destination.x = 80.0f  + ( 130.0f * (i - 4));
            destination.y = 480 - 80 + 8;

            if (
                m_selectedId == static_cast<int>(i)
                || m_hoverId == static_cast<int>(i)
            ) {
                DrawRectangle(destination.x - 8, destination.y - 8, destination.width + 16, destination.height + 16, bgColour);
            }

            DrawTexturePro(iconsTexture, m_icons[i].sourceRect, destination, { 0.0f, 0.0f }, 0.0f, drawColour);
        } else {
            // nothing?
        }
    }
}
