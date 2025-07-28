#include "audiomanager.hpp"

#include "game.hpp"

void AudioManager::OnInitialize() {
    InitAudioDevice();

    int musicCount = m_game->m_gameData.INICount("music");
    for (int i = 0; i < musicCount; ++i) {
        m_audioPaths.emplace_back(m_game->m_gameData.INIString("music", std::to_string(i+1).c_str(), ""));
    }

    for (const auto& it : m_audioPaths) {
        MusicWrapper& bgm = m_game->m_resourceManager.GetMusic(it);
        bgm.state = MUSIC_STATES::STOPPED;

        PlayMusicStream(bgm.track);
        SetMusicVolume(bgm.track, 0.0f);
    }

    int sfxCount = m_game->m_gameData.INICount("sfx");
    for (int i = 0; i < sfxCount; ++i) {
        std::string sfxName = m_game->m_gameData.INIString("sfx", std::to_string(i+1).c_str(), "");
        std::string sfxPath = "resources/" + std::string(m_game->m_gameData.INIString("sfx", sfxName.c_str(), ""));
        m_sfxList.emplace(sfxName, sfxPath);
    }
}

void AudioManager::OnTerminate() {
    // unload all audios
    for (const auto& it : m_audioPaths) {
        MusicWrapper& currentTrack = m_game->m_resourceManager.GetMusic(it);
        UnloadMusicStream(currentTrack.track);
    }

    CloseAudioDevice();
}

void AudioManager::OnUpdate(float deltaTime) {
    // update all audios to keep them in sync
    for (auto& it : m_audioPaths) {
        MusicWrapper& currentTrack = m_game->m_resourceManager.GetMusic(it);

        UpdateMusicStream(currentTrack.track);

        switch (currentTrack.state) {
            case MUSIC_STATES::STOPPED: {
                SetMusicVolume(currentTrack.track, 0.0f);
            } break;
            case MUSIC_STATES::PLAYING: {
                SetMusicVolume(currentTrack.track, 1.0f);
            } break;
            case MUSIC_STATES::FADE_IN: {
                currentTrack.volume += 0.25f * deltaTime;

                if (currentTrack.volume > 1.0f) {
                    currentTrack.volume = 1.0f;
                    currentTrack.state = MUSIC_STATES::PLAYING;
                }

                SetMusicVolume(currentTrack.track, currentTrack.volume);
            } break;
            case MUSIC_STATES::FADE_OUT: {
                currentTrack.volume -= 0.25f * deltaTime;

                if (currentTrack.volume < 0.0f) {
                    currentTrack.volume = 0.0f;
                    currentTrack.state = MUSIC_STATES::STOPPED;
                }

                SetMusicVolume(currentTrack.track, currentTrack.volume);
            }
        }
    }
}

void AudioManager::PlayTrack(std::string trackPath) {
    for (auto& it : m_audioPaths) {
        MusicWrapper& currentTrack = m_game->m_resourceManager.GetMusic(it);

        if (currentTrack.trackPath == trackPath) {
            if (currentTrack.state == MUSIC_STATES::PLAYING) {
                continue;
            }

            if (currentTrack.state == MUSIC_STATES::FADE_IN) {
                continue;
            }

            currentTrack.state = MUSIC_STATES::FADE_IN;

            continue;
        }

        if (
            currentTrack.state == MUSIC_STATES::PLAYING
            || currentTrack.state == MUSIC_STATES::FADE_IN
        ) {
            currentTrack.state = MUSIC_STATES::FADE_OUT;
        }
    }
}

void AudioManager::PlaySFX(std::string sfxName, int min, int max) {
    if (min != 0 && max != 0) {
        int sfxVariation = GetRandomValue(min, max);
        std::string sfxKey = sfxName + std::to_string(sfxVariation);
        if (m_soundPaths.find(sfxKey) == m_soundPaths.end()) {
            m_soundPaths.emplace(sfxKey, LoadSound(m_sfxList[sfxKey].c_str()));
        }
        if (m_soundPaths.find(sfxKey) != m_soundPaths.end()) {
            rlPlaySound(m_soundPaths[sfxKey]);
        }
        return;
    }

    if (m_soundPaths.find(sfxName) == m_soundPaths.end()) {
        m_soundPaths.emplace(sfxName, LoadSound(m_sfxList[sfxName].c_str()));
    }

    if (m_soundPaths.find(sfxName) != m_soundPaths.end()) {
        rlPlaySound(m_soundPaths[sfxName]);
    }
}
