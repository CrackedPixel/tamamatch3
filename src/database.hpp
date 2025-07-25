#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "types.hpp"
#include "raylib.h"

struct Pet {
    PET_STAGES stage;
    PET_STATES state;
    std::unordered_map<PET_ATTRIBUTES, float> attributes;
    std::unordered_map<OUTFIT_SLOTS, int> outfitData;
    std::unordered_map<OUTFIT_SLOTS, Color> outfitTint;
};

struct Item {
    PET_STAGES requiredStage;
    int cursorId;
    int outfitId;
};

struct CursorData {
    std::string texturePath;
};

struct OutfitStageData {
    std::string texturePath;
    Vector2 offset;
    Vector2 scale;
};

struct OutfitData {
    std::unordered_map<PET_STAGES, OutfitStageData> stageData;
};

struct GlobalGameData {
  std::vector<Pet> PetList;
  std::vector<Item> ItemList;
  std::vector<CursorData> Cursors;
};
