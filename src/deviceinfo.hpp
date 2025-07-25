#pragma once

#include "resourcemanager.hpp"

#include "devicebuttons.hpp"

struct DeviceInfo {
    void OnHandleInput();
    void OnUpdate(float deltaTime);
    void OnRender();

    DeviceInfo(ResourceManager& resourceManager);
    ~DeviceInfo() = default;
private:
    ResourceManager& m_resourceManager;
    DeviceButton m_deviceButtons[4];
    int m_hoverId = -1;
    int m_deviceButtonFontSize = 50;
};
