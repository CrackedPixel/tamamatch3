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
};
