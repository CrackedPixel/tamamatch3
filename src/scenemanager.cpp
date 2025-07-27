#include "scenemanager.hpp"

Scene* SceneManager::GetCurrentScene(){
    if (m_currentScene == "") {
        return nullptr;
    }

    return m_scenes[m_currentScene];
}

void SceneManager::OnAddScene(std::string sceneName, Scene* newScene) {
    m_scenes[sceneName] = newScene;
}

bool SceneManager::OnChangeScene(std::string newScene){
    if (m_scenes.find(newScene) == m_scenes.end()) {
        return false;
    }

    if (m_currentScene != "") {
        m_scenes[m_currentScene]->OnTerminate();
    }

    m_currentScene = newScene;
    m_scenes[m_currentScene]->OnInitialize();

    return true;
}

void SceneManager::OnTerminate() {
    for (auto& it : m_scenes) {
        delete it.second;
    }
}
