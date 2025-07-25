#pragma once

#include "devicebuttons.hpp"

struct Game;

struct DeviceInfo {
    bool OnHandleInput();
    void OnUpdate(float deltaTime);
    void OnRender();

    DeviceInfo(Game* game);
    ~DeviceInfo() = default;
private:
    Game* m_game;
    DeviceButton m_deviceButtons[4];
    int m_hoverId = -1;
    int m_deviceButtonFontSize = 50;
};
