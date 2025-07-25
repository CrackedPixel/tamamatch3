#pragma once

#include "database.hpp"
#include "scenemanager.hpp"
#include "resourcemanager.hpp"
#include "audiomanager.hpp"
#include "transitionmanager.hpp"
#include "inputcontroller.hpp"
#include "deviceinfo.hpp"

struct Game {
    public:
        ResourceManager m_resourceManager;
        SceneManager m_sceneManager;
        RenderTexture m_renderTexture;
        InputController m_inputController;
        GlobalGameData m_gameData;
        AudioManager* m_audioManager;
        TransitionManager m_transitionManager;
        DeviceInfo* m_deviceInfo;

    private:
        bool m_mouseHeld = false;
        float m_gameTime = 0.0f;
        Shader m_scanlineShader;
        Vector2 m_lastMousePos = {};
        Vector2 m_lastWindowPos = {};
        Vector2 m_dragOffset = {};
        std::string m_nextSceneName = "";

    public:
        void OnInitialize();
        void OnTerminate();
        void OnGameLoopStart();
        void ChangeScene(std::string newSceneName, bool fadeOut = false);

    private:
        void OnGameLoop();
        void OnHandleInput();
        void OnUpdate();
        void OnRender();
};
