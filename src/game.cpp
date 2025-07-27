#include "game.hpp"

#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
#endif

#include "raylib.h"
#include "scene.hpp"

#include "scene_splash.hpp"
#include "scene_menu.hpp"
#include "scene_credits.hpp"
#include "scene_tama.hpp"

#ifdef PLATFORM_WEB
Game* Game::s_instance = nullptr;
#endif

void Game::OnInitialize() {
#ifndef DEBUG_BUILD
    SetTraceLogLevel(LOG_NONE);
#endif
    #ifdef PLATFORM_DESKTOP
        SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED);
    #endif
    InitWindow(1007, 817, "TamaGacha");

    m_gameData.OnInitialize();

    m_audioManager = new AudioManager(this);
    m_audioManager->OnInitialize();

    m_deviceInfo = new DeviceInfo(this);

#ifdef PLATFORM_DESKTOP
    m_scanlineShader = LoadShader(nullptr, "resources/shaders/scanlines.frag");
#endif

#ifdef PLATFORM_WEB
    m_scanlineShader = LoadShader(nullptr, "resources/shaders/scanlines.web.frag");
#endif

    m_sceneManager.OnAddScene("splash", new SceneSplash(this));
    m_sceneManager.OnAddScene("menu", new SceneMenu(this));
    m_sceneManager.OnAddScene("credits", new SceneCredits(this));
    m_sceneManager.OnAddScene("tama", new SceneTama(this));
#ifdef DEBUG_BUILD
    m_gameData.NewGame();
    m_sceneManager.OnChangeScene("tama");
#else
    m_sceneManager.OnChangeScene("splash");
#endif

    m_renderTexture = LoadRenderTexture(640, 480);

    SetTargetFPS(60);
    #ifndef DEBUG_BUILD
        SetExitKey(KEY_NULL);
    #endif
}

void Game::OnTerminate(){
    delete m_deviceInfo;
    m_deviceInfo = nullptr;

    UnloadShader(m_scanlineShader);

    m_sceneManager.OnTerminate();

    m_audioManager->OnTerminate();
    delete m_audioManager;
    m_audioManager = nullptr;

    m_gameData.OnTerminate();

    rlCloseWindow();
}

void Game::ChangeScene(std::string newSceneName, bool fadeOut, float fadeOutSpeed) {
    if (m_changingScenes) {
        return;
    }

    if (m_nextSceneName != "") {
        return;
    }

    m_changingScenes = true;

    if (fadeOut) {
        if (fadeOutSpeed == 0.0f) {
            m_transitionManager.FadeOut();
        } else {
            m_transitionManager.FadeOut(fadeOutSpeed);
        }
    }

    m_nextSceneName = newSceneName;
}

bool Game::IsChangingScenes() {
    return m_changingScenes;
}

void Game::Quit() {
    m_isRunning = false;
}

void Game::OnHandleInput() {
    if (!m_transitionManager.IsDoneFade()) {
        return;
    }

    m_inputController.OnHandleInput();

    if (IsKeyPressed(KEY_H)) {
        m_renderScanlines = !m_renderScanlines;
    }

    if (
        IsKeyPressed(KEY_W)
        || IsKeyPressed(KEY_A)
        || IsKeyPressed(KEY_UP)
        || IsKeyPressed(KEY_LEFT)
        ) {
        m_inputController.EnableButtonUp();
        return;
    }

    if (
        IsKeyPressed(KEY_S)
        || IsKeyPressed(KEY_D)
        || IsKeyPressed(KEY_DOWN)
        || IsKeyPressed(KEY_RIGHT)
        ) {
        m_inputController.EnableButtonDown();
        return;
    }

    if (
        IsKeyPressed(KEY_ESCAPE)
        || IsKeyPressed(KEY_BACK)
        || IsKeyPressed(KEY_BACKSPACE)
        ) {
        m_inputController.EnableButtonBack();
        return;
    }

    if (
        IsKeyPressed(KEY_ENTER)
        || IsKeyPressed(KEY_SPACE)
        || IsKeyPressed(KEY_KP_ENTER)
        ) {
        m_inputController.EnableButtonSelect();
        return;
    }

    Vector2 mousePosition = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // m_mouseHeld = true;

        // m_lastWindowPos = GetWindowPosition();
        // m_lastMousePos = { GetMouseX() + m_lastWindowPos.x, GetMouseY() + m_lastWindowPos.y };
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // m_mouseHeld = false;
    }

    if (GetMouseWheelMove() < 0.0f) {
        m_inputController.EnableButtonDown();
        return;
    }

    if (GetMouseWheelMove() > 0.0f) {
        m_inputController.EnableButtonUp();
        return;
    }

    bool deviceResult = m_deviceInfo->OnHandleInput();
    if (deviceResult) {
        return;
    }

    Scene* currentScene = m_sceneManager.GetCurrentScene();
    if (currentScene != nullptr) {
        bool sceneResult = currentScene->OnHandleInput(mousePosition);
        if (sceneResult) {
            return;
        }
    }
}

void Game::OnUpdate() {
    float deltaTime = GetFrameTime();
    m_gameTime += deltaTime;

    m_audioManager->OnUpdate(deltaTime);
    m_deviceInfo->OnUpdate(deltaTime);
    m_transitionManager.OnUpdate(deltaTime);

    if (m_nextSceneName != "") {
        if (m_transitionManager.IsDoneFade()) {
            m_inputController.OnHandleInput();
            bool changeResult = m_sceneManager.OnChangeScene(m_nextSceneName);
            m_nextSceneName = "";
            m_changingScenes = false;
            if (!changeResult) {
                // something?
            }
        }
    }

    if (m_mouseHeld) {
        // Vector2 windowCurrentPos = GetWindowPosition();
        // Vector2 mouseCurrentGlobalPos = {
        //     GetMousePosition().x + windowCurrentPos.x,
        //     GetMousePosition().y + windowCurrentPos.y,
        // };

        // m_dragOffset = { mouseCurrentGlobalPos.x - m_lastMousePos.x, mouseCurrentGlobalPos.y - m_lastMousePos.y };
        // // m_lastMousePos.x += m_dragOffset.x;
        // // m_lastMousePos.y += m_dragOffset.y;
        // m_lastMousePos = mouseCurrentGlobalPos;

        // SetWindowPosition(
        //     m_lastWindowPos.x + m_dragOffset.x,
        //     m_lastWindowPos.y + m_dragOffset.y
        // );
    }

    Scene* currentScene = m_sceneManager.GetCurrentScene();
    if (currentScene != nullptr) {
        currentScene->OnUpdate(deltaTime);
    }
}

void Game::OnRender() {
    Scene* currentScene = m_sceneManager.GetCurrentScene();

    BeginDrawing();
#ifdef PLATFORM_WEB
    ClearBackground(BLACK);
#endif
#ifdef PLATFORM_DESKTOP
    ClearBackground(BLANK);
#endif

    if (currentScene != nullptr) {
        BeginTextureMode(m_renderTexture);
        ClearBackground(WHITE);
        currentScene->OnRender();
        m_transitionManager.OnRender();
    }

    if (currentScene != nullptr) {
        currentScene->OnRenderUI();
        EndTextureMode();
    }

    if (currentScene != nullptr) {
        if (m_renderScanlines) {
            BeginShaderMode(m_scanlineShader);
            SetShaderValue(m_scanlineShader, GetShaderLocation(m_scanlineShader, "time"), &m_gameTime, SHADER_UNIFORM_FLOAT);
        }

        DrawTexturePro(m_renderTexture.texture, { 0, 0, 640, -480 }, { 184, 174, 640, 480}, { 0.0f, 0.0f }, 0.0f, WHITE);

        if (m_renderScanlines) {
            EndShaderMode();
        }
    }

    m_deviceInfo->OnRender();

    EndDrawing();
}

void Game::OnRenderUI() {
    Scene* currentScene = m_sceneManager.GetCurrentScene();
    if (currentScene != nullptr) {
        currentScene->OnRenderUI();
    }
}

void Game::OnGameLoopStart(){
    m_isRunning = true;
#ifdef PLATFORM_DESKTOP
    while (m_isRunning) {
        OnGameLoop();
    }
#endif

#ifdef PLATFORM_WEB
    Game::s_instance = this;
    emscripten_set_main_loop(Game::OnGameLoopWeb, 0, 1);
#endif
}

void Game::OnGameLoop(){
    if (WindowShouldClose()) {
        m_isRunning = false;
        return;
    }

    OnHandleInput();
    OnUpdate();
    OnRender();
}
