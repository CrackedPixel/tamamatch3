#pragma once

#include <string>

#include "raylib.h"

enum struct MUSIC_STATES {
    STOPPED,
    PLAYING,
    FADE_IN,
    FADE_OUT,
};

struct MusicWrapper {
    std::string trackPath = "";
    MUSIC_STATES state = MUSIC_STATES::STOPPED;
    Music track = {};
    float volume = 0.0f;

    MusicWrapper() {}
    MusicWrapper(const std::string& path, MUSIC_STATES state, Music track) : trackPath(path), state(state), track(track) {}
};

struct SFXWrapper {
    std::string trackPath = "";
    Sound track = {};

    SFXWrapper() {}
    SFXWrapper(const std::string& path, Sound sound) : trackPath(path), track(sound) {}
};
