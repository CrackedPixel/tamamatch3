#include "scene_collection.hpp"

#include "game.hpp"

void SceneCollection::OnInitialize() {
    m_game->m_transitionManager.FadeIn(0.25f);
}

void SceneCollection::OnUpdate(float deltaTime) {
    if (m_game->IsChangingScenes()) {
        return;
    }

    if (m_game->m_inputController.IsButtonBack) {
        m_game->ChangeScene("tama", true, 0.25f);
        return;
    }

    if (m_game->m_inputController.IsButtonSelect) {
        m_game->m_gameData.activePet = m_selectedId;
        m_game->ChangeScene("tama", true, 0.25f);
        return;
    }
}

void SceneCollection::OnRender() {
    Texture& wallpaperTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentWallpaper());
    Texture& cursorTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentCursorPath(), 0);

    DrawTexture(wallpaperTexture, 0, 0, WHITE);

    for (size_t i = 0; i < m_game->m_gameData.PetList.size(); ++i) {
        DrawPetById(i, m_game->m_gameData.PetList.size() - 1 - i);
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
    Vector2 m_petPosition = { 0, 0 };

    switch (m_game->m_gameData.PetList[petId].stage) {
        case PET_STAGES::EGG: {

        } break;
        case PET_STAGES::NEWBORN: {

        } break;
        case PET_STAGES::TODDLER: {

        } break;
        case PET_STAGES::ADOLESCENT: {

        } break;
        case PET_STAGES::ADULT: {

        } break;
    }
    Pet& petData = m_game->m_gameData.GetCurrentPet();

    Texture& petTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentPetTexturePath());
    Texture& faceTexture = m_game->m_resourceManager.GetTexture("textures/faces.png", 0);

    if (petData.stage == PET_STAGES::EGG) {
        DrawTexturePro(
            petTexture,
            { m_animationStep * m_game->m_gameData.GetCurrentPetWidth(), 0.0f, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { m_petPosition.x, m_petPosition.y, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
        return;
    }


    for (int i = 0; i < static_cast<int>(OUTFIT_SLOTS::COUNT); ++i) {
        auto* outfitData = m_game->m_gameData.GetOutfitDataForSlotItem(static_cast<OUTFIT_SLOTS>(i));
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

            DrawTexturePro(outfitTexture, outfitData->texturePosition, { m_petPosition.x + offsetPos->x, m_petPosition.y + offsetPos->y, outfitData->texturePosition.width, outfitData->texturePosition.height }, { 0, 0 }, 0, outfitTint);
        }
    }

    DrawTexturePro(
        petTexture,
        { m_animationStep * m_game->m_gameData.GetCurrentPetWidth(), 0.0f, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
        { m_petPosition.x, m_petPosition.y, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
        { 0.0f, 0.0f },
        0.0f,
        m_game->m_gameData.PetTintList[petData.petTint]
    );

    for (int i = 0; i < static_cast<int>(OUTFIT_SLOTS::COUNT); ++i) {
        auto* outfitData = m_game->m_gameData.GetOutfitDataForSlotItem(static_cast<OUTFIT_SLOTS>(i));
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

            DrawTexturePro(outfitTexture, outfitData->texturePosition, { m_petPosition.x + offsetPos->x, m_petPosition.y + offsetPos->y, outfitData->texturePosition.width, outfitData->texturePosition.height }, { 0, 0 }, 0, outfitTint);
        }
    }

    if (petData.stage == PET_STAGES::EGG) {
        return;
    }

    // FACES
    DrawTexturePro(
        faceTexture,
        {m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
        { m_petPosition.x + m_game->m_gameData.GetCurrentFaceOffsets().x, m_petPosition.y + m_game->m_gameData.GetCurrentFaceOffsets().y, 64, 64},
        { 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}
