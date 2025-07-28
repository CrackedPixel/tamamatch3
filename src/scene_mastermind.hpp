#pragma once

#include "scene.hpp"

struct MastermindLine {
    int value[4] = { 0, 0, 0, 0 };
};

enum struct MASTERMIND_STATES {
    NEW,
    STARTED,
    WIN,
    LOSE,
};

struct SceneMastermind : public Scene {
    SceneMastermind(Game* game) : Scene(game) {}

    void OnInitialize();
    void OnUpdate(float deltaTime);
    void OnRender();
    void OnRenderUI();

private:
    void ResetBoard();
    void SubmitGuess();

private:
    static constexpr const int MAX_GUESS = 10;
    MastermindLine m_previousGuesses[MAX_GUESS] = {};
    int m_currentGuess = 0;
    int m_selectionId = 0;
    MASTERMIND_STATES m_state = MASTERMIND_STATES::NEW;
};
