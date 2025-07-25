#pragma once

#include <vector>
#include <string>

// #include "audioinfo.hpp"
#include "resourcemanager.hpp"

struct AudioManager {
    void OnInitialize();
    void OnTerminate();
    void OnUpdate(float deltaTime);
    void PlayTrack(std::string trackPath);

    AudioManager(ResourceManager& resourceManager) : m_resourceManager(resourceManager) {}
private:
    ResourceManager& m_resourceManager;
    std::vector<std::string> m_audioPaths;
};
