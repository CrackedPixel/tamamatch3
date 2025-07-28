#pragma once

#include <vector>
#include <unordered_map>

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

struct FoodSpot {
    FOOD_TYPES foodId = FOOD_TYPES::BUG1;
    float currentTime = 0.0f;
    Vector2 position = {};

    FoodSpot(FOOD_TYPES foodId, float currentTime, Vector2 position) : foodId(foodId), currentTime(currentTime), position(position) {}
};

struct StinkySpot {
    float currentTime = 0.0f;
    Vector2 position = {};

    StinkySpot(Vector2 position) : currentTime(0.0f), position(position) {}
};

struct SceneTama;

struct TamaPetAI {
    TamaPetAI(Game* game, SceneTama* sceneTama) : m_game(game), m_tama(sceneTama) {}

    void OnInitialize();
    void OnUpdate(float deltaTime);
    bool OnHandleInput(Vector2 mousePosition);
    void OnRender();
    void OnRenderUI();

public:
    rlRectangle GetPetPosition();
    void SpawnNewInteractSpot(CURSOR_TYPES cursorId, Vector2 position);
    void SpawnNewFoodSpot(CURSOR_TYPES cursorId, Vector2 position);
    void SpawnNewStinkySpot(Vector2 position);
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
    Game* m_game = nullptr;
    SceneTama* m_tama = nullptr;
    float m_currentTimer = 0.0f;
    int m_lastFrameCounter = 0;
    int m_animationStep = 0;
    int m_petDirection = 1;
    float m_petSpeed = 40.0f;
    float m_poopTimer = 0.0f;
    bool m_showStats = false;
    int m_popupItemSelection = -1;
    int m_popupCategorySelection = 0;
    Vector2 m_petPosition = { 320.0f, 240.0f };
    Vector2 m_petTarget = m_petPosition;
    rlRectangle m_petBounds = { 32.0f, 100.0f, 640 - 32, 480 - 100 - 64 };
    std::vector<InteractSpot> m_interactSpotList = {};
    std::vector<FoodSpot> m_foodSpotList = {};
    std::vector<PoopPile> m_poopPileList = {};
    std::vector<StinkySpot> m_stinkySpotList = {};
};
