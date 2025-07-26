#pragma once

#include <unordered_map>
#include <vector>

#include "raylib.h"
#include "cini.h"

#include "enums.hpp"
#include "types.hpp"
#include "utils.hpp"

struct GlobalGameData {
    int activePet = 0;
    CURSOR_TYPES activeCursor = CURSOR_TYPES::NORMAL;
    float gameSpeed = 1.0f;
    std::vector<Pet> PetList = {};
    std::vector<Item> ItemList = {};
    std::vector<OutfitData> OutfitList = {};
    std::unordered_map<CURSOR_TYPES, rlRectangle> Cursors = {};
    std::vector<Color> PetTintList = {};
    std::unordered_map<PET_STAGES, Vector2> PetFaceOffsets = {};
    std::unordered_map<PET_STATES, rlRectangle> PetFaces = {};
    std::vector<std::string> WallpaperList;
    HCINI iniFile;

    void OnInitialize() {
        iniFile = cini_create("resources/config.ini");

        const int wallpaperCount = cini_geti(iniFile, "wallpaper", "count", 0);
        for (int i = 0; i < wallpaperCount; ++i) {
          const char* wallpaperPath = cini_gets(iniFile, "wallpaper", std::to_string(i+1).c_str(), "");
          WallpaperList.emplace_back(wallpaperPath);
        }

        const int petTintCount = cini_geti(iniFile, "pet_tint", "count", 0);
        for (int i = 0; i < petTintCount; ++i) {
            const char* tint = cini_gets(iniFile, "pet_tint", std::to_string(i+1).c_str(), "0 0 0");
            PetTintList.emplace_back(Utils::ColorFromString(tint));
        }

        PetFaces.emplace(PET_STATES::HEALTHY, Utils::RectFromString(cini_gets(iniFile, "pet_faces", "healthy", "0 0 64 64")));
        PetFaces.emplace(PET_STATES::SAD, Utils::RectFromString(cini_gets(iniFile, "pet_faces", "sad", "0 0 64 64")));
        PetFaces.emplace(PET_STATES::SICK, Utils::RectFromString(cini_gets(iniFile, "pet_faces", "sick", "0 0 64 64")));
        PetFaces.emplace(PET_STATES::ANGRY, Utils::RectFromString(cini_gets(iniFile, "pet_faces", "angry", "0 0 64 64")));
        PetFaces.emplace(PET_STATES::DED, Utils::RectFromString(cini_gets(iniFile, "pet_faces", "ded", "0 0 64 64")));

        Cursors.emplace(CURSOR_TYPES::NORMAL, Utils::RectFromString(cini_gets(iniFile, "cursors", "normal", "0 0 32 32")));
        Cursors.emplace(CURSOR_TYPES::ILLNESS, Utils::RectFromString(cini_gets(iniFile, "cursors", "illness", "0 0 32 32")));

        AddNewPet();
    }

    void OnTerminate() {
        cini_free(iniFile);
    }

    void AddNewPet() {
        PetList.emplace_back(PetTintList);
    }

    rlRectangle& GetCurrentCursorCoords() {
        if (Cursors.find(activeCursor) == Cursors.end()) {
            return Cursors[CURSOR_TYPES::NORMAL];
        }

        return Cursors[activeCursor];
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

    const char* GetCurrentPetTexturePath() {
        switch (PetList[activePet].stage) {
            case PET_STAGES::EGG: return "textures/tadpole0.png";
            case PET_STAGES::NEWBORN: return "textures/tadpole1.png";
            case PET_STAGES::TODDLER: return "textures/tadpole2.png";
            case PET_STAGES::ADOLESCENT: return "textures/tadpole3.png";
            case PET_STAGES::ADULT: return "textures/tadpole4.png";
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

    float GetCurrentPetHeight() {
        switch (PetList[activePet].stage) {
            case PET_STAGES::EGG: return 32;
            case PET_STAGES::NEWBORN: return 64;
            case PET_STAGES::TODDLER: return 64;
            case PET_STAGES::ADOLESCENT: return 64;
            case PET_STAGES::ADULT: return 64;
            default: return 0.0f;
        }
    }

    std::string& GetCurrentWallpaper() {
        return WallpaperList[PetList[activePet].wallpaperId];
    }
};
