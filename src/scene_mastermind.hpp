#pragma once

#include "scene.hpp"

struct MastermindLine {
    int value[4] = { 0, 0, 0, 0 };
};

struct MastermindResult {
    int rightColour = 0;
    int rightColourAndSpot = 0;
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
    static constexpr const char* TEXTURE_PATH = "textures/mastermind.png";
    const rlRectangle m_eggTexturePos = { 96, 0, 32, 32 };
    const rlRectangle m_angledPadTexturePos = { 0, 0, 70, 26 };
    const rlRectangle m_lilyTexturePos = { 64, 32, 64, 64 };
    MASTERMIND_STATES m_state = MASTERMIND_STATES::NEW;
    MastermindResult m_previousResults[MAX_GUESS] = {};
    MastermindLine m_previousGuesses[MAX_GUESS] = {};
    MastermindLine m_correctAnswer = {};
    Color m_guessColours[5] = { { 229, 181, 160, 255 }, { 203, 253, 138, 255 }, { 187, 250, 254, 255 }, { 245, 247, 165, 255 }, { 203, 203, 203, 255 } };
    int m_currentGuess = 0;
    int m_selectionId = 0;
};
