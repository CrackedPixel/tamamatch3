#pragma once

#include <vector>

#include "raylib.h"

struct Game;

enum struct ICON_ACTION_TYPE {
    STATS,
    CLEAN,
    CLEAN_TANK,
    MINIGAMES,
    BANDAID,
    INVENTORY,
    TOY,
    CAMERA,
    SETTINGS,
};

struct TamaIcon {
    // std::string texturePath;
    ICON_ACTION_TYPE actionType;
    rlRectangle sourceRect;
};

struct TamaUI {
    TamaUI(Game* game) : m_game(game) {}
    void OnInitialize();
    void OnTerminate();
    void OnUpdate(float deltaTime);
    void OnHandleInput();
    void OnRender();

private:
    static constexpr const char* TEXTURE_PATH = "textures/icons.png";
    Game* m_game = nullptr;
    int m_hoverId = 0;
    std::vector<TamaIcon> m_icons;
};
