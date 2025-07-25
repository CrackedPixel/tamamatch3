#pragma once

#include <unordered_map>
#include <vector>

#include "raylib.h"

#include "enums.hpp"
#include "types.hpp"

struct GlobalGameData {
    int activePet = 0;
    int activeCursor = 0;
    float gameSpeed = 1.0f;
    std::vector<Pet> PetList = {};
    std::vector<Item> ItemList = {};
    std::vector<OutfitData> OutfitList = {};
    std::vector<CursorData> Cursors = {};
    std::vector<Color> PetTintList = {};
    std::unordered_map<PET_STAGES, Vector2> PetFaceOffsets = {};
    std::unordered_map<PET_STATES, rlRectangle> PetFaces = {};
    std::vector<std::string> WallpaperList;

    void OnInitialize() {
        WallpaperList.emplace_back("textures/castle.png");

        PetTintList.emplace_back(WHITE);
        PetTintList.emplace_back(RED);

        PetFaces.emplace(PET_STATES::HEALTHY, rlRectangle{ 64 * 0, 0, 64, 64 });
        PetFaces.emplace(PET_STATES::SAD, rlRectangle{ 64 * 1, 0, 64, 64 });
        PetFaces.emplace(PET_STATES::SICK, rlRectangle{ 64 * 2, 0, 64, 64 });
        PetFaces.emplace(PET_STATES::ANGRY, rlRectangle{ 64 * 3, 0, 64, 64 });
        PetFaces.emplace(PET_STATES::DED, rlRectangle{ 64 * 4, 0, 64, 64 });

        AddNewPet();
    }

    void AddNewPet() {
        PetList.emplace_back(PetTintList);
    }

    Pet& GetCurrentPet() {
        return PetList[activePet];
    }

    rlRectangle& GetCurrentFace() {
        switch (PetList[activePet].state) {
            case PET_STATES::SAD: return PetFaces[PET_STATES::SAD];
            case PET_STATES::SICK: return PetFaces[PET_STATES::SICK];
            case PET_STATES::ANGRY: return PetFaces[PET_STATES::ANGRY];
            case PET_STATES::DED: return PetFaces[PET_STATES::DED];
            default: return PetFaces[PET_STATES::HEALTHY];
        }
    }

    Vector2& GetCurrentFaceOffsets() {
        return PetFaceOffsets[PetList[activePet].stage];
    }

    float GetCurrentPetWidth() {
        switch (PetList[activePet].stage) {
            case PET_STAGES::EGG: return 32;
            case PET_STAGES::NEWBORN: return 64;
            case PET_STAGES::TODDLER: return 128;
            case PET_STAGES::ADOLESCENT: return 128;
            case PET_STAGES::ADULT: return 128;
            default: return 0.0f;
        }
    }

    std::string& GetCurrentWallpaper() {
        return WallpaperList[PetList[activePet].wallpaperId];
    }
};
