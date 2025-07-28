#pragma once

#include <vector>
#include <unordered_map>
#include <string>

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

enum struct POPUP_TYPES {
    NONE,
    STATS,
    PAUSE_MENU,
    INVENTORY,
    EVOLVE,
};

enum struct INVENTORY_PAGES {
    FOOD,
    HATS,
    GLASSES,
    BACK,
    ACC1,
    ACC2,
    COUNT,
};

struct SceneTama;

struct TamaUI {
    TamaUI(Game* game, SceneTama* sceneTama) : m_game(game), m_sceneTama(sceneTama) {}
    void OnInitialize();
    void OnTerminate();
    void OnUpdate(float deltaTime);
    bool OnHandleInput(rlRectangle petPosition);
    void OnRenderUI();

public:
    bool IsUIShown();

private:
    void DrawPetAtSpot(rlRectangle destination);

private:
    static constexpr const char* ICONS_TEXTURE_PATH = "textures/icons.png";
    static constexpr const int OFFSET_X = 80;
    static constexpr const int GAP_X = 105;
    static constexpr const int ICONS_PER_ROW = 5;
    Game* m_game = nullptr;
    SceneTama* m_sceneTama = nullptr;
    int m_hoverId = -1;
    int m_selectedId = 0;
    bool m_hideUI = false;
    std::vector<TamaIcon> m_icons;
    std::unordered_map<std::string, std::string> m_promptsList;
    Color m_barsColour = LIGHTGRAY;
    std::vector<const char*> m_inventoryPageTitles = { "Food", "Hats", "Glasses", "Backs", "Acc 1", "Acc 2" };
    INVENTORY_PAGES m_currentInventoryPage = INVENTORY_PAGES::FOOD;
    int m_currentInventoryTab = 0;
    int m_currentInventorySlot = 0;

public:
    POPUP_TYPES m_popupMenu = POPUP_TYPES::NONE;
};
