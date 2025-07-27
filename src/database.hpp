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

        const int wallpaperCount = INICount("wallpaper");
        for (int i = 0; i < wallpaperCount; ++i) {
            WallpaperList.emplace_back(INIString("wallpaper", std::to_string(i+1).c_str(), ""));
        }

        const int petTintCount = INICount("pet_tint");
        for (int i = 0; i < petTintCount; ++i) {
            PetTintList.emplace_back(Utils::ColorFromString(INIString("pet_tint", std::to_string(i+1).c_str(), "0 0 0")));
        }

        PetFaces.emplace(PET_STATES::HEALTHY, Utils::RectFromString(INIString("pet_faces", "healthy", "0 0 64 64")));
        PetFaces.emplace(PET_STATES::SAD, Utils::RectFromString(INIString("pet_faces", "sad", "0 0 64 64")));
        PetFaces.emplace(PET_STATES::SICK, Utils::RectFromString(INIString("pet_faces", "sick", "0 0 64 64")));
        PetFaces.emplace(PET_STATES::ANGRY, Utils::RectFromString(INIString("pet_faces", "angry", "0 0 64 64")));
        PetFaces.emplace(PET_STATES::DED, Utils::RectFromString(INIString("pet_faces", "ded", "0 0 64 64")));

        PetFaceOffsets.emplace(PET_STAGES::NEWBORN, Utils::Vector2FromString(INIString("pet_face_offsets", "newborn", "0 0")));
        PetFaceOffsets.emplace(PET_STAGES::TODDLER, Utils::Vector2FromString(INIString("pet_face_offsets", "toddler", "0 0")));
        PetFaceOffsets.emplace(PET_STAGES::ADOLESCENT, Utils::Vector2FromString(INIString("pet_face_offsets", "adolescent", "0 0")));
        PetFaceOffsets.emplace(PET_STAGES::ADULT, Utils::Vector2FromString(INIString("pet_face_offsets", "adult", "0 0")));

        Cursors.emplace(CURSOR_TYPES::NORMAL, Utils::RectFromString(INIString("cursors", "normal", "0 0 32 32")));
        Cursors.emplace(CURSOR_TYPES::ILLNESS, Utils::RectFromString(INIString("cursors", "illness", "0 0 32 32")));
        Cursors.emplace(CURSOR_TYPES::TOY, Utils::RectFromString(INIString("cursors", "ball", "0 0 32 32")));
        Cursors.emplace(CURSOR_TYPES::DIRTY, Utils::RectFromString(INIString("cursors", "bath", "0 0 32 32")));
        Cursors.emplace(CURSOR_TYPES::TANKDIRTY, Utils::RectFromString(INIString("cursors", "tank", "0 0 32 32")));

        Cursors.emplace(CURSOR_TYPES::STINKY, Utils::RectFromString(INIString("cursors", "stinky", "0 0 32 32")));
    }

    void OnTerminate() {
        cini_free(iniFile);
    }

    const char* INIString(const char* header, const char* key, const char* defaultValue) {
        return cini_gets(iniFile, header, key, defaultValue);
    }

    int INICount(const char* header) {
        return INIInt(header, "count", 0);
    }

    int INIInt(const char* header, const char* key, int defaultValue) {
        return cini_geti(iniFile, header, key, defaultValue);
    }

    float INIFloat(const char* header, const char* key, float defaultValue) {
        return cini_getf(iniFile, header, key, defaultValue);
    }

    void NewGame() {
        PetList.clear();

        AddNewPet();

        // do we need more?
    }

    void LoadGame() {
        // TODO: load from file
    }

    void AddNewPet() {
        PetList.emplace_back(PetTintList);
    }

    void EvolveCurrentPet() {
        if (PetList[activePet].stage < PET_STAGES::ADULT) {
            switch (PetList[activePet].stage) {
                case PET_STAGES::EGG: {
                    PetList[activePet].stage = PET_STAGES::NEWBORN;
                } break;
                case PET_STAGES::NEWBORN:{
                    PetList[activePet].stage = PET_STAGES::TODDLER;
                } break;
                case PET_STAGES::TODDLER:{
                    PetList[activePet].stage = PET_STAGES::ADOLESCENT;
                } break;
                case PET_STAGES::ADOLESCENT:{
                    PetList[activePet].stage = PET_STAGES::ADULT;
                } break;
                default: break;
            }

            PetList[activePet].attributes[PET_ATTRIBUTES::HUNGER] = 0.0f;
            PetList[activePet].attributes[PET_ATTRIBUTES::HAPPINESS] = 1.0f;
            PetList[activePet].attributes[PET_ATTRIBUTES::BOREDOM] = 0.0f;
            PetList[activePet].attributes[PET_ATTRIBUTES::HYGIENE] = 1.0f;
            PetList[activePet].attributes[PET_ATTRIBUTES::TANKHYGIENE] = 1.0f;
            PetList[activePet].attributes[PET_ATTRIBUTES::ILLNESS] = 1.0f;
            PetList[activePet].attributes[PET_ATTRIBUTES::GROWTH] = 0.0f;
            PetList[activePet].attributes[PET_ATTRIBUTES::HP] = 1.0f;

            PetList[activePet].state = PET_STATES::HEALTHY;
        }
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
            case PET_STAGES::ADULT: return 128;
            default: return 0.0f;
        }
    }

    std::string& GetCurrentWallpaper() {
        return WallpaperList[PetList[activePet].wallpaperId];
    }
};
