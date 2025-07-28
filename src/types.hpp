#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "raylib.h"
#include "enums.hpp"

struct PetInventoryItem {
    int outfitId;
    int outfitTint;

    PetInventoryItem(int outfitId, int outfitTint) : outfitId(outfitId), outfitTint(outfitTint) {}
};

struct GumballItem {
    ITEM_TYPES itemType;
    int id = 0;
    int tint = 0;
    int slot = 0;
};

struct Pet {
    PET_STAGES stage = PET_STAGES::EGG;
    PET_STATES state = PET_STATES::HEALTHY;
    std::unordered_map<PET_ATTRIBUTES, float> attributes = {};
    std::unordered_map<OUTFIT_SLOTS, int> outfitId = {};
    std::unordered_map<OUTFIT_SLOTS, int> outfitTint = {};
    std::unordered_map<OUTFIT_SLOTS, std::vector<PetInventoryItem>> outfitInventory;
    std::unordered_map<FOOD_TYPES, int> foodInventory;
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

    void AddNewInventoryItem(GumballItem newItem) {
        switch (newItem.itemType) {
            case ITEM_TYPES::FOOD: {
                foodInventory[static_cast<FOOD_TYPES>(newItem.id)] += 1;
            } break;
            case ITEM_TYPES::OUTFIT: {
                bool alreadyHas = false;
                for (const auto& it : outfitInventory[static_cast<OUTFIT_SLOTS>(newItem.slot)]) {
                    if (
                        it.outfitId == newItem.id
                        && it.outfitTint == newItem.tint
                    ) {
                        alreadyHas = true;
                    }
                }

                if (!alreadyHas) {
                    outfitInventory[static_cast<OUTFIT_SLOTS>(newItem.slot)].emplace_back(PetInventoryItem{ newItem.id, newItem.tint });
                }
            } break;
            default: return;
        }
    }
};

struct OutfitData {
    std::string texturePath = "";
    rlRectangle texturePosition = {};
    bool adultOnly = false;
    Vector2 offsetNewborn = {};
    Vector2 offsetToddler = {};
    Vector2 offsetAdolescent = {};
    Vector2 offsetAdult = {};
    bool isColourable = false;
    bool isBehind = false;

    OutfitData(
        const std::string& texPath,
        const rlRectangle& rect,
        const int adultOnly,
        const Vector2& offsetNewborn,
        const Vector2& offsetToddler,
        const Vector2& offsetAdolescent,
        const Vector2& offsetAdult,
        int i1,
        int i2
        ) : texturePath(texPath),
        texturePosition(rect),
        adultOnly(adultOnly),
        offsetNewborn(offsetNewborn),
        offsetToddler(offsetToddler),
        offsetAdolescent(offsetAdolescent),
        offsetAdult(offsetAdult),
        isColourable(i1),
        isBehind(i2)
    {}

    Vector2* GetOffsetForStage(PET_STAGES stage) {
        switch (stage) {
            case PET_STAGES::EGG: {
                return nullptr;
            } break;

            case PET_STAGES::NEWBORN: {
                return &offsetNewborn;
            } break;

            case PET_STAGES::TODDLER: {
                return &offsetToddler;
            } break;

            case PET_STAGES::ADOLESCENT: {
                return &offsetAdolescent;
            } break;

            case PET_STAGES::ADULT: {
                return &offsetAdult;
            } break;
        }
    }
};
