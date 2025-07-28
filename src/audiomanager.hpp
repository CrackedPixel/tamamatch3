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
    void StopMusic();

    AudioManager(Game* game) : m_game(game) {}
private:
    Game* m_game = nullptr;
    std::vector<std::string> m_audioPaths = {};
    std::unordered_map<std::string, Sound> m_soundPaths = {};
    std::unordered_map<std::string, std::string> m_sfxList = {};

public:
    bool m_sfxEnabled = true;
    bool m_musicEnabled = true;
};
