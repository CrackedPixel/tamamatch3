#include "game.hpp"

#ifdef PLATFORM_WEB
#endif

#include "raylib.h"
#include "scene.hpp"

#include "scene_splash.hpp"
#include "scene_tama.hpp"

void Game::OnInitialize() {
    // SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED);
    InitWindow(1007, 817, "Tamamatch3");

    m_gameData.OnInitialize();

    m_audioManager = new AudioManager(m_resourceManager);
    m_audioManager->OnInitialize();

    m_deviceInfo = new DeviceInfo(this);

    m_scanlineShader = LoadShader(nullptr, "resources/shaders/scanlines.frag");

    m_sceneManager.OnAddScene("splash", new SceneSplash(this));
    m_sceneManager.OnAddScene("tama", new SceneTama(this));
    m_sceneManager.OnChangeScene("splash");

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

    rlCloseWindow();
}

void Game::ChangeScene(std::string newSceneName, bool fadeOut) {
    if (m_nextSceneName != "") {
        return;
    }

    if (fadeOut) {
        m_transitionManager.FadeOut();
    }

    m_nextSceneName = newSceneName;
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

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // m_mouseHeld = true;

        // m_lastWindowPos = GetWindowPosition();
        // m_lastMousePos = { GetMouseX() + m_lastWindowPos.x, GetMouseY() + m_lastWindowPos.y };
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // m_mouseHeld = false;
    }

    bool deviceResult = m_deviceInfo->OnHandleInput();
    if (deviceResult) {
        return;
    }

    Scene* currentScene = m_sceneManager.GetCurrentScene();
    if (currentScene != nullptr) {
        bool sceneResult = currentScene->OnHandleInput(GetMousePosition());
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
            m_sceneManager.OnChangeScene(m_nextSceneName);
            m_nextSceneName = "";
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
    ClearBackground(RED);

    BeginTextureMode(m_renderTexture);
    ClearBackground(WHITE);

    if (currentScene != nullptr) {
        BeginTextureMode(m_renderTexture);

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
#ifdef PLATFORM_DESKTOP
    while (!WindowShouldClose()) {
        OnGameLoop();
    }
#endif

#ifdef PLATFORM_WEB
#endif
}

void Game::OnGameLoop(){
    OnHandleInput();
    OnUpdate();
    OnRender();
}
