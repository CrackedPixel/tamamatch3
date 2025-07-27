#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "raylib.h"

struct Game;

struct AudioManager {
    void OnInitialize();
    void OnTerminate();
    void OnUpdate(float deltaTime);
    void PlayTrack(std::string trackPath);
    void PlaySFX(std::string sfxPath, int min = 0, int max = 0);

    AudioManager(Game* game) : m_game(game) {}
private:
    Game* m_game;
    std::vector<std::string> m_audioPaths;
    std::unordered_map<std::string, Sound> m_soundPaths;
};
