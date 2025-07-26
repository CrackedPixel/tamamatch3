#include "audiomanager.hpp"

#include "game.hpp"

void AudioManager::OnInitialize() {
    InitAudioDevice();

    int musicCount = m_game->m_gameData.INICount("music");
    for (int i = 0; i < musicCount; ++i) {
        m_audioPaths.emplace_back(m_game->m_gameData.INIString("music", std::to_string(i).c_str(), ""));
    }

    for (const auto& it : m_audioPaths) {
        MusicWrapper& bgm = m_game->m_resourceManager.GetMusic(it);
        bgm.state = MUSIC_STATES::STOPPED;

        PlayMusicStream(bgm.track);
        SetMusicVolume(bgm.track, 0.0f);
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
        if (
            currentTrack.state == MUSIC_STATES::PLAYING
            || currentTrack.state == MUSIC_STATES::FADE_IN
        ) {
            currentTrack.state = MUSIC_STATES::FADE_OUT;
        }

        if (currentTrack.trackPath == trackPath) {
            currentTrack.state = MUSIC_STATES::FADE_IN;
        }
    }
}

void AudioManager::PlaySFX(std::string sfxPath) {

}
