#pragma once

#include <vector>
#include <string>

struct Game;

struct AudioManager {
    void OnInitialize();
    void OnTerminate();
    void OnUpdate(float deltaTime);
    void PlayTrack(std::string trackPath);
    void PlaySFX(std::string sfxPath);

    AudioManager(Game* game) : m_game(game) {}
private:
    Game* m_game;
    std::vector<std::string> m_audioPaths;
};
