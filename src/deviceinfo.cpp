#include "deviceinfo.hpp"
#include "game.hpp"

DeviceInfo::DeviceInfo(Game* game) : m_game(game) {
    m_deviceButtons[0].position = { 255 + (64*2 * 0), 654, 49*2, 49*2 };
    m_deviceButtons[1].position = { 255 + (64*2 * 1), 654, 49*2, 49*2 };
    m_deviceButtons[2].position = { 255 + (64*2 * 2), 654, 49*2, 49*2 };
    m_deviceButtons[3].position = { 255 + (64*2 * 3), 654, 49*2, 49*2 };

    m_deviceButtons[0].displayText = "🡅";
    m_deviceButtons[1].displayText = "⮌";
    m_deviceButtons[2].displayText = "➕";
    m_deviceButtons[3].displayText = "🡇";
}

void DeviceInfo::OnUpdate(float deltaTime) {
    Vector2 mousePosition = GetMousePosition();

    for (int i = 0; i < 4; ++i) {
        if (CheckCollisionPointRec(mousePosition, m_deviceButtons[i].position)) {
            m_hoverId = i;
            return;
        }
    }

    m_hoverId = -1;
}

bool DeviceInfo::OnHandleInput() {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return false;
    }

    switch (m_hoverId) {
        // case -1: return false;
        case 0: {
            m_game->m_inputController.EnableButtonUp();
            return true;
        } break;
        case 1: {
            m_game->m_inputController.EnableButtonBack();
            return true;
        } break;
        case 2: {
            m_game->m_inputController.EnableButtonSelect();
            return true;
        } break;
        case 3: {
            m_game->m_inputController.EnableButtonDown();
            return true;
        } break;
        default: return false;
    }
}

void DeviceInfo::OnRender() {
    Texture& deviceTexture = m_game->m_resourceManager.GetTexture("textures/device.png");
    Texture& deviceButtonTexture = m_game->m_resourceManager.GetTexture("textures/device_button.png");
    Font& deviceFont = m_game->m_resourceManager.GetFont("fonts/symbola.ttf", m_deviceButtonFontSize, "🡇🡅➕⮌");

    DrawTexture(deviceTexture, 0, 0, WHITE);

    Color buttonColour;
    for (int i = 0; i < 4; ++i) {
        buttonColour = m_hoverId == i ?  LIGHTGRAY : WHITE;
        DrawTexturePro(deviceButtonTexture, { 0, 0, 49, 49 }, m_deviceButtons[i].position, { 0.0f, 0.0f }, 0.0f, buttonColour);
    }

    for (int i = 0; i < 4; ++i) {
        rlDrawTextEx(deviceFont, m_deviceButtons[i].displayText.c_str(), { m_deviceButtons[i].position.x + (m_deviceButtonFontSize * 0.6f), m_deviceButtons[i].position.y + (m_deviceButtonFontSize * 0.5f) }, m_deviceButtonFontSize, 0.0f, WHITE);
    }
}
