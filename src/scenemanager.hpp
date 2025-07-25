#pragma once

#include <unordered_map>
#include <string>

#include "scene.hpp"

struct SceneManager {
    std::string m_currentScene = "";
    std::unordered_map<std::string, Scene*> m_scenes;

    Scene* GetCurrentScene();

    void OnAddScene(std::string sceneName, Scene* newScene);
    void OnChangeScene(std::string newScene);
    void OnTerminate();
};
