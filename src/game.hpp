#pragma once

#include "database.hpp"
#include "scenemanager.hpp"
#include "resourcemanager.hpp"
#include "audiomanager.hpp"
#include "transitionmanager.hpp"
#include "inputcontroller.hpp"
#include "deviceinfo.hpp"

struct Game {
#ifdef PLATFORM_WEB
    public:
        static Game* s_instance;
        static void OnGameLoopWeb() {
            if (s_instance != nullptr) {
                s_instance->OnGameLoop();
            }
        }
#endif

    public:
        ResourceManager m_resourceManager = {};
        SceneManager m_sceneManager = {};
        RenderTexture m_renderTexture = {};
        InputController m_inputController = {};
        GlobalGameData m_gameData = {};
        TransitionManager m_transitionManager = {};
        AudioManager* m_audioManager = nullptr;
        DeviceInfo* m_deviceInfo = nullptr;

    private:
        bool m_isRunning = false;
        bool m_mouseHeld = false;
        bool m_renderScanlines = true;
        bool m_changingScenes = false;
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
        void ChangeScene(std::string newSceneName, bool fadeOut = false, float fadeOutSpeed = 0.0f);
        bool IsChangingScenes();
        void Quit();

    private:
        void OnGameLoop();
        void OnHandleInput();
        void OnUpdate();
        void OnRender();
        void OnRenderUI();
};
