#include "tamapetai.hpp"

#include "game.hpp"
#include "scene_tama.hpp"

void TamaPetAI::OnInitialize() {
    m_lastFrameCounter = 0;
    m_currentTimer = 0.0f;

    m_game->m_audioManager->PlaySFX("hatch");
}

void TamaPetAI::OnUpdate(float deltaTime) {
    Pet& petData = m_game->m_gameData.GetCurrentPet();

    if (m_interactSpotList.size() > 0) {
        for (int i = m_interactSpotList.size()-1; i >= 0; --i) {
            m_interactSpotList[i].currentTime += deltaTime;

            if (m_interactSpotList[i].currentTime >= 1.0f) {
                m_interactSpotList.erase(m_interactSpotList.begin() + i);
                continue;
            }
        }
    }

    if (m_stinkySpotList.size() > 0) {
        for (int i = m_stinkySpotList.size()-1; i >= 0; --i) {
            m_stinkySpotList[i].currentTime += deltaTime;

            if (m_stinkySpotList[i].currentTime >= 1.0f) {
                m_stinkySpotList.erase(m_stinkySpotList.begin() + i);
                continue;
            }
        }
    }

    if (petData.state == PET_STATES::DED) {
        m_animationStep = 1;
        return;
    }

    if (petData.state == PET_STATES::EVOLVE) {
        m_animationStep = 1;
        return;
    }

    if (m_tama->GetTamaUI()->m_popupMenu == POPUP_TYPES::NONE) {
        petData.attributes[PET_ATTRIBUTES::GROWTH] += deltaTime * m_game->m_gameData.gameSpeed;
        m_poopTimer += deltaTime * m_game->m_gameData.gameSpeed;
    }

    m_currentTimer += deltaTime * m_game->m_gameData.gameSpeed;
    if (static_cast<int>(m_currentTimer / 0.5f) > m_lastFrameCounter) {
        m_lastFrameCounter = static_cast<int>(m_currentTimer / 0.5f);
        ProcessAI();
    }

    ProcessMovement(deltaTime);
}

bool TamaPetAI::OnHandleInput(Vector2 mousePosition) {
    if (IsKeyPressed(KEY_F1)) {
        m_showStats = !m_showStats;
        return true;
    }

    if (m_tama->GetTamaUI()->m_popupMenu != POPUP_TYPES::NONE) {
        return false;
    }

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return false;
    }

    if (m_game->m_gameData.activeCursor != CURSOR_TYPES::NORMAL) {
        return false;
    }

    rlRectangle PetBoundsGlobal = m_petBounds;
    PetBoundsGlobal.x += 184;
    PetBoundsGlobal.y += 174;
    PetBoundsGlobal.width += 184;
    PetBoundsGlobal.height += 174;

    if (CheckCollisionPointRec(mousePosition, PetBoundsGlobal)) {
        Vector2 newTarget = mousePosition;
        newTarget.x -= 184;
        newTarget.y -= 174;
        SetNewPetTarget(newTarget);

        return true;
    }

    return false;
}

void TamaPetAI::OnRender() {
    Pet& petData = m_game->m_gameData.GetCurrentPet();

    Texture& petTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentPetTexturePath());
    Texture& faceTexture = m_game->m_resourceManager.GetTexture("textures/faces.png");

    if (petData.stage == PET_STAGES::EGG) {
        DrawTexturePro(
            petTexture,
            { (m_animationStep % MAX_ANIMATION_STEPS) * m_game->m_gameData.GetCurrentPetWidth(), GetOffsetFromState(), m_petDirection * m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { m_petPosition.x, m_petPosition.y, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
        return;
    }

    if (petData.state == PET_STATES::DED) {
        for (int i = 0; i < static_cast<int>(OUTFIT_SLOTS::COUNT); ++i) {
            if (petData.outfitId[static_cast<OUTFIT_SLOTS>(i)] > 0) {
                auto& outfitData = m_game->m_gameData.OutfitList[static_cast<OUTFIT_SLOTS>(i)].at(petData.outfitId[static_cast<OUTFIT_SLOTS>(i)]-1);
                if (!outfitData.isBehind) {
                    continue;
                }

                if (outfitData.adultOnly && petData.stage != PET_STAGES::ADULT) {
                    continue;
                }

                Vector2* offsetPos = outfitData.GetOffsetForStage(petData.stage);
                if (offsetPos != nullptr) {
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath);
                    Color& outfitTint = outfitData.isColourable ? m_game->m_gameData.OutfitTintList[petData.outfitTint[static_cast<OUTFIT_SLOTS>(i)]-1] : m_game->m_gameData.OutfitTintList[0];

                    if (m_petDirection == 1) {
                        DrawTexturePro(outfitTexture, { outfitData.texturePosition.x, outfitData.texturePosition.y, outfitData.texturePosition.width, -outfitData.texturePosition.height }, { m_petPosition.x + offsetPos->x, m_petPosition.y + offsetPos->y, outfitData.texturePosition.width, outfitData.texturePosition.height }, { 0, 0 }, 0, outfitTint);
                    } else {
                        DrawTexturePro(outfitTexture, { outfitData.texturePosition.x, outfitData.texturePosition.y, -outfitData.texturePosition.width, -outfitData.texturePosition.height }, { m_petPosition.x - offsetPos->x + (m_game->m_gameData.GetCurrentPetWidth() - outfitData.texturePosition.width), m_petPosition.y + offsetPos->y, outfitData.texturePosition.width, outfitData.texturePosition.height }, { 0, 0 }, 0, outfitTint);
                    }
                }
            }
        }

        DrawTexturePro(
           petTexture,
           { (m_animationStep % MAX_ANIMATION_STEPS) * m_game->m_gameData.GetCurrentPetWidth(), GetOffsetFromState(), m_petDirection * m_game->m_gameData.GetCurrentPetWidth(), -m_game->m_gameData.GetCurrentPetHeight() },
           { m_petPosition.x, m_petPosition.y, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
           { 0.0f, 0.0f },
           0.0f,
           m_game->m_gameData.PetTintList[petData.petTint]
        );

        for (int i = 0; i < static_cast<int>(OUTFIT_SLOTS::COUNT); ++i) {
            if (petData.outfitId[static_cast<OUTFIT_SLOTS>(i)] > 0) {
                auto& outfitData = m_game->m_gameData.OutfitList[static_cast<OUTFIT_SLOTS>(i)].at(petData.outfitId[static_cast<OUTFIT_SLOTS>(i)]-1);
                if (outfitData.isBehind) {
                    continue;
                }

                if (outfitData.adultOnly && petData.stage != PET_STAGES::ADULT) {
                    continue;
                }

                Vector2* offsetPos = outfitData.GetOffsetForStage(petData.stage);
                if (offsetPos != nullptr) {
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath);
                    Color& outfitTint = outfitData.isColourable ? m_game->m_gameData.OutfitTintList[petData.outfitTint[static_cast<OUTFIT_SLOTS>(i)]-1] : m_game->m_gameData.OutfitTintList[0];

                    if (m_petDirection == 1) {
                        DrawTexturePro(outfitTexture, { outfitData.texturePosition.x, outfitData.texturePosition.y, outfitData.texturePosition.width, -outfitData.texturePosition.height }, { m_petPosition.x + offsetPos->x, m_petPosition.y + offsetPos->y, outfitData.texturePosition.width, outfitData.texturePosition.height }, { 0, 0 }, 0, outfitTint);
                    } else {
                        DrawTexturePro(outfitTexture, { outfitData.texturePosition.x, outfitData.texturePosition.y, -outfitData.texturePosition.width, -outfitData.texturePosition.height }, { m_petPosition.x - offsetPos->x + (m_game->m_gameData.GetCurrentPetWidth() - outfitData.texturePosition.width), m_petPosition.y + offsetPos->y, outfitData.texturePosition.width, outfitData.texturePosition.height }, { 0, 0 }, 0, outfitTint);
                    }
                }
            }
        }
    } else {

        for (int i = 0; i < static_cast<int>(OUTFIT_SLOTS::COUNT); ++i) {
            if (petData.outfitId[static_cast<OUTFIT_SLOTS>(i)] > 0) {
                auto& outfitData = m_game->m_gameData.OutfitList[static_cast<OUTFIT_SLOTS>(i)].at(petData.outfitId[static_cast<OUTFIT_SLOTS>(i)]-1);
                if (!outfitData.isBehind) {
                    continue;
                }

                if (outfitData.adultOnly && petData.stage != PET_STAGES::ADULT) {
                    continue;
                }

                Vector2* offsetPos = outfitData.GetOffsetForStage(petData.stage);
                if (offsetPos != nullptr) {
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath);
                    Color& outfitTint = outfitData.isColourable ? m_game->m_gameData.OutfitTintList[petData.outfitTint[static_cast<OUTFIT_SLOTS>(i)]-1] : m_game->m_gameData.OutfitTintList[0];

                    if (m_petDirection == 1) {
                        DrawTexturePro(outfitTexture, outfitData.texturePosition, { m_petPosition.x + offsetPos->x, m_petPosition.y + offsetPos->y, outfitData.texturePosition.width, outfitData.texturePosition.height }, { 0, 0 }, 0, outfitTint);
                    } else {
                        DrawTexturePro(outfitTexture, { outfitData.texturePosition.x, outfitData.texturePosition.y, -outfitData.texturePosition.width, outfitData.texturePosition.height }, { m_petPosition.x - offsetPos->x + (m_game->m_gameData.GetCurrentPetWidth() - outfitData.texturePosition.width), m_petPosition.y + offsetPos->y, outfitData.texturePosition.width, outfitData.texturePosition.height }, { 0, 0 }, 0, outfitTint);
                    }
                }
            }
        }

        DrawTexturePro(
            petTexture,
            { (m_animationStep % MAX_ANIMATION_STEPS) * m_game->m_gameData.GetCurrentPetWidth(), GetOffsetFromState(), m_petDirection * m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { m_petPosition.x, m_petPosition.y, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { 0.0f, 0.0f },
            0.0f,
            m_game->m_gameData.PetTintList[petData.petTint]
        );

        for (int i = 0; i < static_cast<int>(OUTFIT_SLOTS::COUNT); ++i) {
            if (petData.outfitId[static_cast<OUTFIT_SLOTS>(i)] > 0) {
                auto& outfitData = m_game->m_gameData.OutfitList[static_cast<OUTFIT_SLOTS>(i)].at(petData.outfitId[static_cast<OUTFIT_SLOTS>(i)]-1);
                if (outfitData.isBehind) {
                    continue;
                }

                if (outfitData.adultOnly && petData.stage != PET_STAGES::ADULT) {
                    continue;
                }

                Vector2* offsetPos = outfitData.GetOffsetForStage(petData.stage);
                if (offsetPos != nullptr) {
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath);
                    Color& outfitTint = outfitData.isColourable ? m_game->m_gameData.OutfitTintList[petData.outfitTint[static_cast<OUTFIT_SLOTS>(i)]-1] : m_game->m_gameData.OutfitTintList[0];

                    if (m_petDirection == 1) {
                        DrawTexturePro(outfitTexture, outfitData.texturePosition, { m_petPosition.x + offsetPos->x, m_petPosition.y + offsetPos->y, outfitData.texturePosition.width, outfitData.texturePosition.height }, { 0, 0 }, 0, outfitTint);
                    } else {
                        DrawTexturePro(outfitTexture, { outfitData.texturePosition.x, outfitData.texturePosition.y, -outfitData.texturePosition.width, outfitData.texturePosition.height }, { m_petPosition.x - offsetPos->x + (m_game->m_gameData.GetCurrentPetWidth() - outfitData.texturePosition.width), m_petPosition.y + offsetPos->y, outfitData.texturePosition.width, outfitData.texturePosition.height }, { 0, 0 }, 0, outfitTint);
                    }
                }
            }
        }
    }

    if (petData.stage == PET_STAGES::EGG) {
        return;
    }

    // FACES
    if (m_petDirection == -1) {
        if (petData.state == PET_STATES::DED) {
            DrawTexturePro(
                faceTexture,
                {m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetCurrentFace().width, -m_game->m_gameData.GetCurrentFace().height},
                { m_petPosition.x + m_game->m_gameData.GetCurrentFaceOffsets().x + m_game->m_gameData.GetCurrentPetWidth() - 42, m_petPosition.y + m_game->m_gameData.GetCurrentFaceOffsets().y, 64, 64},
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        } else {
            DrawTexturePro(
                faceTexture,
                {m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
                { m_petPosition.x - m_game->m_gameData.GetCurrentFaceOffsets().x + m_game->m_gameData.GetCurrentPetWidth() - 42, m_petPosition.y + m_game->m_gameData.GetCurrentFaceOffsets().y, 64, 64},
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }
    } else {
        if (petData.state == PET_STATES::DED) {
            DrawTexturePro(
                faceTexture,
                {m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetCurrentFace().width, -m_game->m_gameData.GetCurrentFace().height},
                { m_petPosition.x + m_game->m_gameData.GetCurrentFaceOffsets().x, m_petPosition.y + m_game->m_gameData.GetCurrentFaceOffsets().y, 64, 64},
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        } else {
            DrawTexturePro(
                faceTexture,
                {m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
                { m_petPosition.x + m_game->m_gameData.GetCurrentFaceOffsets().x, m_petPosition.y + m_game->m_gameData.GetCurrentFaceOffsets().y, 64, 64},
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }
    }
}

void TamaPetAI::OnRenderUI() {
    Texture& cursorTexture = m_game->m_resourceManager.GetTexture("textures/cursors.png");
    for (const auto& it : m_interactSpotList) {
        DrawTexturePro(cursorTexture, m_game->m_gameData.Cursors[it.cursorId], { it.position.x, it.position.y - (it.currentTime * 64.0f), 32, 32 }, { 0, 0 }, 0.0f, ColorAlpha(WHITE, 1.0f - it.currentTime));
    }

    Texture& poopTexture = m_game->m_resourceManager.GetTexture("textures/misc.png");
    for (const auto& it : m_poopPileList) {
        DrawTexturePro(poopTexture, { 0, 0, 32, 32 }, { it.position.x, it.position.y, 32, 32 }, { 0, 0 }, 0.0f, WHITE);
    }

    Texture& stinkyTexture = m_game->m_resourceManager.GetTexture("textures/stinky.png");
    for (const auto& it : m_stinkySpotList) {
        DrawTexturePro(stinkyTexture, { (m_animationStep % 3) * 32.0f, 0, 32, 39 }, { it.position.x, it.position.y - (it.currentTime * 64.0f), 32, 32 }, { 0, 0 }, 0.0f, ColorAlpha(WHITE, 1.0f - it.currentTime));
    }

    if (!m_showStats) {
        return;
    }

    Pet& petData = m_game->m_gameData.GetCurrentPet();

    rlDrawText(TextFormat("Growth: %.1f\nHP: %.1f\nHunger: %.1f\nBoredom: %.1f\nHappiness: %.1f\nHygiene: %.1f\nTank: %.1f\nHealth: %.1f",
      (petData.attributes[PET_ATTRIBUTES::GROWTH] / m_game->m_gameData.GetCurrentPetGrowthTime()),
      petData.attributes[PET_ATTRIBUTES::HP],
      petData.attributes[PET_ATTRIBUTES::HUNGER],
      petData.attributes[PET_ATTRIBUTES::BOREDOM],
      petData.attributes[PET_ATTRIBUTES::HAPPINESS],
      petData.attributes[PET_ATTRIBUTES::HYGIENE],
      petData.attributes[PET_ATTRIBUTES::TANKHYGIENE],
      petData.attributes[PET_ATTRIBUTES::ILLNESS]
    ), 50, 100, 20, BLACK);
}

rlRectangle TamaPetAI::GetPetPosition() {
    return { m_petPosition.x, m_petPosition.y, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() };
}

float TamaPetAI::GetOffsetFromState() {
    return 0.0f;
}

void TamaPetAI::ProcessAI() {
    m_animationStep += 1;

    if (m_animationStep > (MAX_ANIMATION_STEPS * 2)) {
        m_animationStep = 0;
    }

    if (m_lastFrameCounter % 10 == 0) {
        AIMovement();
    }

    ProcessAttributes();
}

void TamaPetAI::ProcessMovement(float deltaTime) {
    if (m_petPosition.x < m_petTarget.x) {
        m_petDirection = -1;

        m_petPosition.x += deltaTime * m_petSpeed;
        if (m_petPosition.x > m_petTarget.x) {
            m_petPosition.x = m_petTarget.x;
        }
    }

    if (m_petPosition.x > m_petTarget.x) {
        m_petDirection = 1;

        m_petPosition.x -= deltaTime * m_petSpeed;
        if (m_petPosition.x < m_petTarget.x) {
            m_petPosition.x = m_petTarget.x;
        }
    }

    if (m_petPosition.y < m_petTarget.y) {
        m_petPosition.y += deltaTime * m_petSpeed;
        if (m_petPosition.y > m_petTarget.y) {
            m_petPosition.y = m_petTarget.y;
        }
    }

    if (m_petPosition.y > m_petTarget.y) {
        m_petPosition.y -= deltaTime * m_petSpeed;
        if (m_petPosition.y < m_petTarget.y) {
            m_petPosition.y = m_petTarget.y;
        }
    }
}

void TamaPetAI::ProcessAttributes() {
    if (m_tama->GetTamaUI()->m_popupMenu != POPUP_TYPES::NONE) {
        return;
    }

    Pet& petData = m_game->m_gameData.GetCurrentPet();
    if (petData.stage == PET_STAGES::ADULT) {
        return;
    }

    if (petData.stage == PET_STAGES::EGG) {
        if (petData.attributes[PET_ATTRIBUTES::GROWTH] >= m_game->m_gameData.GetCurrentPetGrowthTime()) {
            petData.state = PET_STATES::EVOLVE;

            if (m_tama->GetTamaUI()->m_popupMenu == POPUP_TYPES::NONE) {
                m_tama->GetTamaUI()->m_popupMenu = POPUP_TYPES::EVOLVE;
            }
        }

        return;
    }

#ifdef DEBUG_BUILD
    constexpr const float multiplySpeed = 0.001f;
#else
    constexpr const float multiplySpeed = 0.0001f;
#endif
    petData.attributes[PET_ATTRIBUTES::HUNGER] += GetRandomValue(10, 20) * multiplySpeed;
    petData.attributes[PET_ATTRIBUTES::BOREDOM] += GetRandomValue(15, 40) * multiplySpeed;
    petData.attributes[PET_ATTRIBUTES::HYGIENE] -= GetRandomValue(15, 50) * multiplySpeed;
    petData.attributes[PET_ATTRIBUTES::ILLNESS] -= GetRandomValue(5, 15) * multiplySpeed;

    Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::HUNGER]);
    Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::BOREDOM]);
    Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::HYGIENE]);
    Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::ILLNESS]);

    float healthChange = 0.0f;
    if (petData.attributes[PET_ATTRIBUTES::HUNGER] >= 0.25f) {
        healthChange -= 0.05f * (petData.attributes[PET_ATTRIBUTES::HUNGER] * 0.5f);
    } else {
        healthChange += 0.025f;
    }

    if (petData.attributes[PET_ATTRIBUTES::HAPPINESS] <= 0.55f) {
        healthChange -= 0.05f * ((1 - petData.attributes[PET_ATTRIBUTES::HAPPINESS]) );
    } else {
        healthChange += 0.025f;
    }

    if (petData.attributes[PET_ATTRIBUTES::HYGIENE] <= 0.75f) {
        healthChange -= 0.05f * ((1 - petData.attributes[PET_ATTRIBUTES::HYGIENE]) * 0.5f);
    } else {
        healthChange += 0.025f;
    }

    if (petData.attributes[PET_ATTRIBUTES::TANKHYGIENE] <= 0.75f) {
        healthChange -= 0.5f * 0.05f * ((1 - petData.attributes[PET_ATTRIBUTES::TANKHYGIENE]) * 0.25f);
    } else {
        healthChange += 0.025f * 0.5f;
    }

    if (petData.attributes[PET_ATTRIBUTES::ILLNESS] <= 0.75f) {
        healthChange -= 0.05f * ((1 - petData.attributes[PET_ATTRIBUTES::ILLNESS]) * 0.5f);
    } else {
        healthChange += 0.025f;
    }

    if (m_poopPileList.size() > 0) {
        petData.attributes[PET_ATTRIBUTES::TANKHYGIENE] -= 0.025f * m_poopPileList.size();
    } else {
        petData.attributes[PET_ATTRIBUTES::TANKHYGIENE] += 0.05f;
    }
    Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::TANKHYGIENE]);

    petData.attributes[PET_ATTRIBUTES::HP] += healthChange;
    Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::HP]);

    if (healthChange < 0.0f) {
        petData.attributes[PET_ATTRIBUTES::HAPPINESS] += (healthChange * 0.5f);
    } else {
        petData.attributes[PET_ATTRIBUTES::HAPPINESS] += healthChange;
    }
    Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::HAPPINESS]);

    if (petData.attributes[PET_ATTRIBUTES::HP] <= 0.0f) {
        petData.state = PET_STATES::DED;
        m_game->m_audioManager->PlaySFX("die", 1, 3);

        return;
    }

    if (m_poopTimer >= 5.0f) {
        m_poopTimer = 0.0f;
        if (m_animationStep < 5) {
            m_game->m_audioManager->PlaySFX("poo");
            SpawnNewPoopPile({
                static_cast<float>(GetRandomValue(m_petBounds.x, m_petBounds.width)),
                static_cast<float>(GetRandomValue(m_petBounds.y, m_petBounds.height)),
                32,
                32
            });
        }
    }

    if (m_animationStep % 3 == 0 && petData.attributes[PET_ATTRIBUTES::HUNGER] > 0.45f) {
        m_game->m_audioManager->PlaySFX("hungry");
    }

    if ((m_animationStep % 2 == 0) && petData.attributes[PET_ATTRIBUTES::HYGIENE] < 0.50f) {
        if (m_petDirection == -1) {
            SpawnNewStinkySpot({ m_petPosition.x + (m_game->m_gameData.GetCurrentPetWidth() - 32), m_petPosition.y + (m_game->m_gameData.GetCurrentPetHeight() * 0.25f) });
        } else {
            SpawnNewStinkySpot({ m_petPosition.x, m_petPosition.y + (m_game->m_gameData.GetCurrentPetHeight() * 0.25f) });
        }
    }

    if (petData.state == PET_STATES::HEALTHY) {
        if (petData.attributes[PET_ATTRIBUTES::GROWTH] >= m_game->m_gameData.GetCurrentPetGrowthTime()) {
            petData.state = PET_STATES::EVOLVE;

            if (m_tama->GetTamaUI()->m_popupMenu == POPUP_TYPES::NONE) {
                m_tama->GetTamaUI()->m_popupMenu = POPUP_TYPES::EVOLVE;
            }

            return;
        }

        // if (petData.attributes[PET_ATTRIBUTES::HUNGER] > 0.45f) {
        //     petData.state = PET_STATES::HUNGRY;
        //     return;
        // }

        if (petData.attributes[PET_ATTRIBUTES::BOREDOM] > 0.35f) {
            petData.state = PET_STATES::ANGRY;
            return;
        }

        if (petData.attributes[PET_ATTRIBUTES::HAPPINESS] < 0.65f) {
            petData.state = PET_STATES::SAD;
            return;
        }

        if (petData.attributes[PET_ATTRIBUTES::ILLNESS] < 0.65f) {
            petData.state = PET_STATES::SICK;
            return;
        }
    }

    if (petData.state == PET_STATES::ANGRY) {
        if (petData.attributes[PET_ATTRIBUTES::BOREDOM] < 0.35f) {
            petData.state = PET_STATES::HEALTHY;
            return;
        }
    }

    if (petData.state == PET_STATES::SAD) {
        if (petData.attributes[PET_ATTRIBUTES::HAPPINESS] > 0.65f) {
            petData.state = PET_STATES::HEALTHY;
            return;
        }
    }

    if (petData.state == PET_STATES::SICK) {
        m_game->m_audioManager->PlayTrack("music/tamagotchi_sick.ogg");

        if (petData.attributes[PET_ATTRIBUTES::ILLNESS] > 0.65f) {
            petData.state = PET_STATES::HEALTHY;
            return;
        }
    }
}

void TamaPetAI::AIMovement() {
    // if (GetRandomValue(0, 1) == 1) {
    //     m_petTarget.x = m_petPosition.x + GetRandomValue(64, 256);
    // } else {
    //     m_petTarget.x = m_petPosition.x - GetRandomValue(64, 256);
    // }

    if (GetRandomValue(0, 10) == 1) {
        m_petTarget.x = m_petPosition.x;
        m_petTarget.y = m_petPosition.y;

        return;
    }

    SetNewPetTarget(
        GetRandomValue(m_petBounds.x, m_petBounds.width),
        GetRandomValue(m_petBounds.y, m_petBounds.height)
    );
}

void TamaPetAI::SetNewPetTarget(int newX, int newY) {
    m_petTarget.x = newX;
    m_petTarget.y = newY;

    if (m_petTarget.x < m_petBounds.x) {
        m_petTarget.x = m_petBounds.x;
    }

    if (m_petTarget.y < m_petBounds.y) {
        m_petTarget.y = m_petBounds.y;
    }

    if (m_petTarget.x > m_petBounds.width - m_game->m_gameData.GetCurrentPetWidth()) {
        m_petTarget.x = m_petBounds.width - m_game->m_gameData.GetCurrentPetWidth();
    }

    if (m_petTarget.y > m_petBounds.height) {
        m_petTarget.y = m_petBounds.height;
    }
}

void TamaPetAI::SetNewPetTarget(Vector2 destination) {
    SetNewPetTarget(destination.x, destination.y);
}

void TamaPetAI::SpawnNewInteractSpot(CURSOR_TYPES cursorId, Vector2 position) {
    position.x += GetRandomValue(-10, 10);
    m_interactSpotList.emplace_back(InteractSpot{ cursorId, 0.0f, position });
}

void TamaPetAI::SpawnNewStinkySpot(Vector2 position) {
    position.x += GetRandomValue(-10, 10);
    m_stinkySpotList.emplace_back(position);
}

void TamaPetAI::SpawnNewPoopPile(rlRectangle position) {
    m_poopPileList.emplace_back(PoopPile{ 1.0f, position });
}
