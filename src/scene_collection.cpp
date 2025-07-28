#include "scene_collection.hpp"

#include "game.hpp"

void SceneCollection::OnInitialize() {
    m_selectedId = 0;
    m_game->m_transitionManager.FadeIn(0.5f);
}

void SceneCollection::OnUpdate(float deltaTime) {
    if (m_game->IsChangingScenes()) {
        return;
    }

    if (m_game->m_inputController.IsButtonBack) {
        m_game->ChangeScene("tama", true, 1.0f);
        return;
    }

    if (m_game->m_inputController.IsButtonSelect) {
        if (m_game->m_gameData.HasAllAdults()) {
            if (m_selectedId == 0) {
                m_game->m_gameData.AddNewPet();
                m_game->ChangeScene("tama", true, 1.0f);
                return;
            }

            m_game->m_gameData.activePet = m_game->m_gameData.PetList.size() - m_selectedId;
            m_game->ChangeScene("tama", true, 1.0f);
            return;
        }
        m_game->m_gameData.activePet = m_game->m_gameData.PetList.size() - 1 - m_selectedId;
        m_game->ChangeScene("tama", true, 1.0f);
        return;
    }

    if (m_game->m_inputController.IsButtonUp) {
        m_selectedId -= 1;

        if (m_selectedId < 0) {
            m_selectedId = 0;
        }

        return;
    }

    if (m_game->m_inputController.IsButtonDown) {
        m_selectedId += 1;

        if (m_game->m_gameData.HasAllAdults()) {
            if (m_selectedId == static_cast<int>(m_game->m_gameData.PetList.size())+1) {
                m_selectedId = static_cast<int>(m_game->m_gameData.PetList.size());
            }

            return;
        }

        if (m_selectedId == static_cast<int>(m_game->m_gameData.PetList.size())) {
            m_selectedId = static_cast<int>(m_game->m_gameData.PetList.size())-1;
        }

        return;
    }
}

void SceneCollection::OnRender() {
    Texture& wallpaperTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentWallpaper());
    Texture& cursorTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentCursorPath(), 0);

    DrawTexture(wallpaperTexture, 0, 0, WHITE);

    if (m_game->m_gameData.HasAllAdults()) {
        DrawPetById(0, 0);

        for (size_t i = 0; i < m_game->m_gameData.PetList.size(); ++i) {
            DrawPetById(i+1, m_game->m_gameData.PetList.size() -1 - i);
        }
    } else {
        for (size_t i = 0; i < m_game->m_gameData.PetList.size(); ++i) {
            DrawPetById(i, m_game->m_gameData.PetList.size() -1 - i);
        }
    }


    Vector2 mousePosition = GetMousePosition();
    mousePosition.x -= 184;
    mousePosition.y -= 174;

    if (CheckCollisionPointRec(mousePosition, { 0, 0, 640, 480 })) {
#ifdef PLATFORM_DESKTOP
        rlHideCursor();
#endif

        DrawTexturePro(
            cursorTexture,
            m_game->m_gameData.GetCurrentCursorCoords(),
            { mousePosition.x, mousePosition.y, 32, 32 },
            { 0, 0 },
            0.0f,
            WHITE
        );
    } else {
#ifdef PLATFORM_DESKTOP
        rlShowCursor();
#endif
    }
}

void SceneCollection::DrawPetById(int slot, int petId) {
    int m_animationStep = 0;
    Vector2 m_petPosition = { 100.0f + ((slot % 4) * 128.0f), 50.0f + ((slot / 4) * 128.0f) };
    Vector2 m_petOffset = { 0, 0 };

    Texture& lilyTexture = m_game->m_resourceManager.GetTexture("textures/misc.png");
    Color drawColour = m_selectedId == slot ?  Color{ 172, 211, 148, 255 } : WHITE;
    DrawTexturePro(lilyTexture, { 64, 64, 64, 64 }, { m_petPosition.x, m_petPosition.y, 64, 64 }, { 0, 0 }, 0.0f, drawColour);

    if (slot == 0 && m_game->m_gameData.HasAllAdults()) {
        rlDrawText("New", m_petPosition.x+5, m_petPosition.y-26, 30, BLACK);
        return;
    }

    switch (m_game->m_gameData.PetList[petId].stage) {
        case PET_STAGES::EGG: {
            m_petOffset.x += 16;
            m_petOffset.y += 8;
        } break;
        case PET_STAGES::NEWBORN: {
            m_petOffset.y -= 4;
            m_animationStep = 1;
        } break;
        case PET_STAGES::TODDLER: {
            m_petOffset.x -= 16;
            m_animationStep = 1;
        } break;
        case PET_STAGES::ADOLESCENT: {
            m_petOffset.x -= 16;
            m_animationStep = 1;
        } break;
        case PET_STAGES::ADULT: {
            m_petOffset.x -= 25;
            m_petOffset.y -= 59;
        } break;
    }
    Pet& petData = m_game->m_gameData.GetPet(petId);

    Texture& petTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetPetTexturePath(petId));
    Texture& faceTexture = m_game->m_resourceManager.GetTexture("textures/faces.png", 0);

    if (petData.stage == PET_STAGES::EGG) {
        DrawTexturePro(
            petTexture,
            { m_animationStep * m_game->m_gameData.GetPetWidth(petId), 0.0f, m_game->m_gameData.GetPetWidth(petId), m_game->m_gameData.GetPetHeight(petId) },
            { m_petPosition.x + m_petOffset.x, m_petPosition.y + m_petOffset.y, m_game->m_gameData.GetPetWidth(petId), m_game->m_gameData.GetPetHeight(petId) },
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
        return;
    }


    for (int i = 0; i < static_cast<int>(OUTFIT_SLOTS::COUNT); ++i) {
        auto* outfitData = m_game->m_gameData.GetOutfitDataForSlotItem(static_cast<OUTFIT_SLOTS>(i), petId);
        if (outfitData == nullptr) {
            continue;
        }
        if (!outfitData->isBehind) {
            continue;
        }

        if (outfitData->adultOnly && petData.stage != PET_STAGES::ADULT) {
            continue;
        }

        Vector2* offsetPos = outfitData->GetOffsetForStage(petData.stage);
        if (offsetPos != nullptr) {
            Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData->texturePath, 0);
            Color& outfitTint = outfitData->isColourable ? m_game->m_gameData.OutfitTintList[petData.outfitTint[static_cast<OUTFIT_SLOTS>(i)]-1] : m_game->m_gameData.OutfitTintList[0];

            DrawTexturePro(outfitTexture, outfitData->texturePosition, { m_petPosition.x + offsetPos->x + m_petOffset.x, m_petPosition.y + offsetPos->y + m_petOffset.y, outfitData->texturePosition.width, outfitData->texturePosition.height }, { 0, 0 }, 0, outfitTint);
        }
    }

    DrawTexturePro(
        petTexture,
        { m_animationStep * m_game->m_gameData.GetPetWidth(petId), 0.0f, m_game->m_gameData.GetPetWidth(petId), m_game->m_gameData.GetPetHeight(petId) },
        { m_petPosition.x + m_petOffset.x, m_petPosition.y + m_petOffset.y, m_game->m_gameData.GetPetWidth(petId), m_game->m_gameData.GetPetHeight(petId) },
        { 0.0f, 0.0f },
        0.0f,
        m_game->m_gameData.PetTintList[petData.petTint]
    );

    for (int i = 0; i < static_cast<int>(OUTFIT_SLOTS::COUNT); ++i) {
        auto* outfitData = m_game->m_gameData.GetOutfitDataForSlotItem(static_cast<OUTFIT_SLOTS>(i), petId);
        if (outfitData == nullptr) {
            continue;
        }
        if (outfitData->isBehind) {
            continue;
        }

        if (outfitData->adultOnly && petData.stage != PET_STAGES::ADULT) {
            continue;
        }

        Vector2* offsetPos = outfitData->GetOffsetForStage(petData.stage);
        if (offsetPos != nullptr) {
            Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData->texturePath, 0);
            Color& outfitTint = outfitData->isColourable ? m_game->m_gameData.OutfitTintList[petData.outfitTint[static_cast<OUTFIT_SLOTS>(i)]-1] : m_game->m_gameData.OutfitTintList[0];

            DrawTexturePro(outfitTexture, outfitData->texturePosition, { m_petPosition.x + offsetPos->x + m_petOffset.x, m_petPosition.y + offsetPos->y + m_petOffset.y, outfitData->texturePosition.width, outfitData->texturePosition.height }, { 0, 0 }, 0, outfitTint);
        }
    }

    if (petData.stage == PET_STAGES::EGG) {
        return;
    }

    // FACES
    DrawTexturePro(
        faceTexture,
        {m_game->m_gameData.GetFace(petId).x, m_game->m_gameData.GetFace(petId).y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetFace(petId).width, m_game->m_gameData.GetFace(petId).height},
        { m_petPosition.x + m_game->m_gameData.GetFaceOffsets(petId).x + m_petOffset.x, m_petPosition.y + m_game->m_gameData.GetFaceOffsets(petId).y + m_petOffset.y, 64, 64},
        { 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}
