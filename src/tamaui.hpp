#pragma once

#include <vector>

#include "raylib.h"

struct Game;

enum struct ICON_ACTION_TYPE {
    STATS,
    BANDAID,
    TOY,
    CLEAN,
    CLEAN_TANK,

    CAMERA,
    INVENTORY,
    MINIGAMES,
    STORE,
    DISPLAY,
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
    bool OnHandleInput();
    void OnRenderUI();

public:
    bool IsUIShown();

private:
    void DrawPetAtSpot(rlRectangle destination);

private:
    static constexpr const char* TEXTURE_PATH = "textures/icons.png";
    static constexpr const int OFFSET_X = 80;
    static constexpr const int GAP_X = 105;
    static constexpr const int ICONS_PER_ROW = 5;
    Game* m_game = nullptr;
    int m_hoverId = -1;
    int m_selectedId = 0;
    bool hideUI = false;
    std::vector<TamaIcon> m_icons;
};
