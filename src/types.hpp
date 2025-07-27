#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "raylib.h"
#include "enums.hpp"

struct Pet {
    PET_STAGES stage = PET_STAGES::EGG;
    PET_STATES state = PET_STATES::HEALTHY;
    std::unordered_map<PET_ATTRIBUTES, float> attributes = {};
    std::unordered_map<OUTFIT_SLOTS, int> outfitData = {};
    std::unordered_map<OUTFIT_SLOTS, Color> outfitTint = {};
    int petTint = 0;
    int wallpaperId = 0;

    Pet(std::vector<Color> petTintList) {
        attributes[PET_ATTRIBUTES::HUNGER] = 0.0f;
        attributes[PET_ATTRIBUTES::HAPPINESS] = 1.0f;
        attributes[PET_ATTRIBUTES::BOREDOM] = 0.0f;
        attributes[PET_ATTRIBUTES::HYGIENE] = 1.0f;
        attributes[PET_ATTRIBUTES::TANKHYGIENE] = 1.0f;
        attributes[PET_ATTRIBUTES::ILLNESS] = 1.0f;
        attributes[PET_ATTRIBUTES::GROWTH] = 0.0f;
        attributes[PET_ATTRIBUTES::HP] = 1.0f;

        // TODO: randomize?
        petTint = GetRandomValue(0, petTintList.size() - 1);
        wallpaperId = GetRandomValue(0, 1);
    }
};

struct Item {
    PET_STAGES requiredStage;
    int cursorId;
    int outfitId;
};

struct OutfitStageData {
    std::string texturePath;
    rlRectangle texturePosition;
    Vector2 offset;
    Vector2 scale;
};

struct OutfitData {
    std::unordered_map<PET_STAGES, OutfitStageData> stageData = {};
};
