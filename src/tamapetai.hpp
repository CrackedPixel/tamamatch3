#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "raylib.h"
#include "enums.hpp"

struct Game;

struct PoopPile {
    float scale = 1.0f;
    rlRectangle position = {};

    PoopPile(float scale, rlRectangle position) : scale(scale), position(position) {}
};

struct InteractSpot {
    CURSOR_TYPES cursorId = CURSOR_TYPES::NORMAL;
    float currentTime = 0.0f;
    Vector2 position = {};

    InteractSpot(CURSOR_TYPES cursorId, float currentTime, Vector2 position) : cursorId(cursorId), currentTime(currentTime), position(position) {}
};

enum struct POPUP_TYPES {
    NONE,
    PAUSE_MENU,
    INVENTORY,
    EVOLVE,
};

struct TamaPetAI {
    TamaPetAI(Game* game) : m_game(game){}

    void OnInitialize();
    void OnUpdate(float deltaTime);
    bool OnHandleInput(Vector2 mousePosition);
    void OnRender();
    void OnRenderUI();

public:
    rlRectangle GetPetPosition();
    void SpawnNewInteractSpot(CURSOR_TYPES cursorId, Vector2 position);
    void SpawnNewPoopPile(rlRectangle position);
    std::vector<PoopPile>& GetPoopPileList() {
        return m_poopPileList;
    }

private:
    float GetOffsetFromState();
    void ProcessAI();
    void ProcessMovement(float deltaTime);
    void ProcessAttributes();
    void AIMovement();
    void SetNewPetTarget(int newX, int newY);
    void SetNewPetTarget(Vector2 destination);
    void ClampRange(float& value);

private:
    static constexpr const int MAX_ANIMATION_STEPS = 4;
#ifdef DEBUG_BUILD
    static constexpr const int GROWTH_EGG = 1;
    static constexpr const int GROWTH_TADPOLE = 5;
#else
    static constexpr const int GROWTH_EGG = 60;
    static constexpr const int GROWTH_TADPOLE = 600;
#endif
    Game* m_game = nullptr;
    float m_currentTimer = 0.0f;
    int m_lastFrameCounter = 0;
    int m_animationStep = 0;
    int m_petDirection = 1;
    float m_petSpeed = 40.0f;
    float m_poopTimer = 0.0f;
    bool m_showStats = false;
    POPUP_TYPES m_popupMenu = POPUP_TYPES::NONE;
    int m_popupItemSelection = -1;
    int m_popupCategorySelection = 0;
    Vector2 m_petPosition = { 320.0f, 240.0f };
    Vector2 m_petTarget = m_petPosition;
    rlRectangle m_petBounds = { 32.0f, 100.0f, 640 - 32, 480 - 100 - 64 };
    std::vector<InteractSpot> m_interactSpotList = {};
    std::vector<PoopPile> m_poopPileList = {};
    std::unordered_map<std::string, std::string> promptsList;
};
