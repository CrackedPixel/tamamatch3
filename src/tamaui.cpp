#include "tamaui.hpp"

#include "game.hpp"
#include "scene_tama.hpp"

void TamaUI::OnInitialize() {
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::STATS, { 0, 0, 0, 0 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::BANDAID, { 64 * 2, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::TOY, { 64 * 3, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::CLEAN, { 64 * 2, 64, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::CLEAN_TANK, { 64 * 0, 64 * 2, 64, 64 } });

    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::CAMERA, { 64 * 0, 64, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::INVENTORY, { 64 * 1, 64 * 1, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::MINIGAMES, { 64 * 1, 0, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::STORE, { 64 * 3, 64, 64, 64 } });
    m_icons.emplace_back(TamaIcon{ ICON_ACTION_TYPE::DISPLAY, { 0, 0, 64, 64 } });
}

void TamaUI::OnTerminate() {
    m_icons.clear();
}

void TamaUI::OnUpdate(float deltaTime) {
    if (m_game->m_inputController.IsButtonUp) {
        m_selectedId -= 1;

        if (m_selectedId < 0) {
            m_selectedId = m_icons.size() - 1;
        }

        return;
    }

    if (m_game->m_inputController.IsButtonDown) {
        m_selectedId += 1;

        if (m_selectedId == static_cast<int>(m_icons.size())) {
            m_selectedId = 0;
        }

        return;
    }

    if (m_game->m_inputController.IsButtonBack) {
        if (hideUI) {
            hideUI = false;
            return;
        }
    }

    if (m_game->m_inputController.IsButtonSelect) {
        if (hideUI) {
            hideUI = false;
            return;
        }


        // STATS,
        // BANDAID,
        // TOY,
        // CLEAN,
        // CLEAN_TANK,

            // CAMERA,
            // INVENTORY,
            // MINIGAMES,
            // STORE,
            // DISPLAY,

        //     petData.attributes[PET_ATTRIBUTES::HUNGER] += GetRandomValue(10, 20) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::HAPPINESS] -= GetRandomValue(10, 26) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::BOREDOM] += GetRandomValue(15, 40) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::HYGIENE] -= GetRandomValue(15, 50) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::TANKHYGIENE] -= GetRandomValue(10, 26) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::ILLNESS] -= GetRandomValue(5, 15) * multiplySpeed;


        // device select
        switch (m_icons[m_selectedId].actionType) {
            case ICON_ACTION_TYPE::STATS: {
                // TODO: open stats
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            } break;
            case ICON_ACTION_TYPE::BANDAID: {
                // m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                auto& petData = m_game->m_gameData.GetCurrentPet();
                petData.attributes[PET_ATTRIBUTES::ILLNESS] += 0.3f;
                m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::ILLNESS, { m_sceneTama->GetPetAI()->GetPetPosition().x, m_sceneTama->GetPetAI()->GetPetPosition().y });
            } break;
            case ICON_ACTION_TYPE::TOY: {
                // m_game->m_gameData.activeCursor = CURSOR_TYPES::TOY;
                auto& petData = m_game->m_gameData.GetCurrentPet();
                petData.attributes[PET_ATTRIBUTES::BOREDOM] -= 0.5f;
                petData.attributes[PET_ATTRIBUTES::HAPPINESS] += 0.35f;
                // Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::BOREDOM]);
                // Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::HAPPINESS]);
                m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::TOY, { m_sceneTama->GetPetAI()->GetPetPosition().x, m_sceneTama->GetPetAI()->GetPetPosition().y });
            } break;
            case ICON_ACTION_TYPE::CLEAN: {
                // m_game->m_gameData.activeCursor = CURSOR_TYPES::DIRTY;
                auto& petData = m_game->m_gameData.GetCurrentPet();
                petData.attributes[PET_ATTRIBUTES::HYGIENE] += 0.5f;
                m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::DIRTY, { m_sceneTama->GetPetAI()->GetPetPosition().x, m_sceneTama->GetPetAI()->GetPetPosition().y });
            } break;
            case ICON_ACTION_TYPE::CLEAN_TANK: {
                // m_game->m_gameData.activeCursor = CURSOR_TYPES::TANKDIRTY;
                // auto& petData = m_game->m_gameData.GetCurrentPet();
                // petData.attributes[PET_ATTRIBUTES::TANKHYGIENE] += 0.5f;
                // m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::TANKDIRTY, { m_sceneTama->GetPetAI()->GetPetPosition().x, m_sceneTama->GetPetAI()->GetPetPosition().y });
                auto& poopList = m_sceneTama->GetPetAI()->GetPoopPileList();
                if (poopList.size() > 0) {
                    m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::TANKDIRTY, { poopList[0].position.x, poopList[0].position.y });
                    poopList.erase(poopList.begin());
                }
            } break;


            case ICON_ACTION_TYPE::CAMERA: {
                hideUI = !hideUI;

                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            } break;
            case ICON_ACTION_TYPE::INVENTORY: {
                // TODO: open inventory
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            } break;
            case ICON_ACTION_TYPE::MINIGAMES: {
                // TODO: change scene
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            } break;
            case ICON_ACTION_TYPE::STORE: {
                // TODO: open store
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            } break;
            case ICON_ACTION_TYPE::DISPLAY: {
                // TODO: open display
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            } break;

        }

        m_game->m_audioManager->PlaySFX("uiselect");

        return;
    }

    Vector2 mousePosition = GetMousePosition();
    mousePosition.x -= 184;
    mousePosition.y -= 174;

    rlRectangle destination = { 0, 0, 80, 80 };
    for (size_t i = 0; i < m_icons.size(); ++i) {
        if (i < ICONS_PER_ROW) {
            destination.x = OFFSET_X  + ( GAP_X * i) - 8;
            destination.y = 0;

            if (CheckCollisionPointRec(mousePosition, destination)) {
                m_hoverId = i;

                return;
            }
        } else if (i < ICONS_PER_ROW * 2) {
            destination.x = OFFSET_X  + ( GAP_X * (i - ICONS_PER_ROW)) - 8;
            destination.y = 480 - 80;

            if (CheckCollisionPointRec(mousePosition, destination)) {
                m_hoverId = i;

                return;
            }
        }
    }

    m_hoverId = -1;
}

bool TamaUI::OnHandleInput(rlRectangle petPosition) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return false;
    }

    if (m_hoverId == -1) {
        Vector2 mousePosition = GetMousePosition();
        mousePosition.x -= 184;
        mousePosition.y -= 174;

        if (CheckCollisionPointRec(mousePosition, petPosition)) {
            Pet& petData = m_game->m_gameData.GetCurrentPet();
            switch (m_game->m_gameData.activeCursor) {
                case CURSOR_TYPES::SAD: {
                    petData.attributes[PET_ATTRIBUTES::HAPPINESS] += 0.45f;
                    m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::SAD, mousePosition);
                } break;
                case CURSOR_TYPES::DIRTY: {
                    petData.attributes[PET_ATTRIBUTES::HYGIENE] += 0.45f;
                    m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::DIRTY, mousePosition);
                } break;
                case CURSOR_TYPES::ILLNESS: {
                    petData.attributes[PET_ATTRIBUTES::ILLNESS] += 0.45f;
                    m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::ILLNESS, mousePosition);
                } break;
                case CURSOR_TYPES::TOY: {
                    petData.attributes[PET_ATTRIBUTES::BOREDOM] -= 0.45f;
                    m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::TOY, mousePosition);
                } break;
                default: return false;
            }

            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;

            return true;
        }

        if (m_game->m_gameData.activeCursor == CURSOR_TYPES::TANKDIRTY) {
            auto& poopPileList = m_sceneTama->GetPetAI()->GetPoopPileList();
            for (size_t i = 0; i < poopPileList.size(); ++i) {
                if (CheckCollisionPointRec(mousePosition, poopPileList[i].position)) {
                    m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                    m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::TANKDIRTY, {
                        poopPileList[i].position.x, poopPileList[i].position.y
                    });
                    poopPileList.erase(poopPileList.begin() + i);
                    return true;
                }
            }
        }

        return false;
    }

    // enum struct CURSOR_TYPES {
    //     NORMAL,
    //     INVALID,
    //     FOOD,
    //     SAD,
    //     BORED,
    //     DIRTY,
    //     TANKDIRTY,
    //     ILLNESS,
    //     TOY,
    // };

    // mouse
    switch (m_icons[m_hoverId].actionType) {
        case ICON_ACTION_TYPE::STATS: {
            // TODO: open stats
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::BANDAID: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::ILLNESS;
        } break;
        case ICON_ACTION_TYPE::TOY: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::TOY;
        } break;
        case ICON_ACTION_TYPE::CLEAN: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::DIRTY;
        } break;
        case ICON_ACTION_TYPE::CLEAN_TANK: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::TANKDIRTY;
        } break;

        case ICON_ACTION_TYPE::CAMERA: {
            hideUI = !hideUI;

            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::INVENTORY: {
            // TODO: open inventory
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::MINIGAMES: {
            // TODO: change scene
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::STORE: {
            // TODO: open settings
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
        case ICON_ACTION_TYPE::DISPLAY: {
            // TODO: open display
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
        } break;
    }

    m_game->m_audioManager->PlaySFX("uiselect");

    return true;
}

void TamaUI::OnRenderUI() {
    if (hideUI) {
        return;
    }

    Texture& iconsTexture = m_game->m_resourceManager.GetTexture(TEXTURE_PATH);

    Color bgColour;
    rlRectangle destination = { 0.0f, 0.0f, 64, 64 };

    DrawRectangle(0, 0, 640, 80, LIGHTGRAY);
    DrawRectangle(0, 480 - 80, 640, 80, LIGHTGRAY);

    for (size_t i = 0; i < m_icons.size(); ++i) {
        bgColour = m_hoverId == static_cast<int>(i) ? YELLOW : RED;
        if (i < ICONS_PER_ROW) {
            destination.x = OFFSET_X  + ( GAP_X * i);
            destination.y = 8;

            if (
                m_selectedId == static_cast<int>(i)
                || m_hoverId == static_cast<int>(i)
            ) {
                DrawRectangle(destination.x - 8, destination.y - 8, destination.width + 16, destination.height + 16, bgColour);
            }

            if (m_icons[i].actionType == ICON_ACTION_TYPE::STATS) {
                DrawPetAtSpot(destination);
            } else {
                DrawTexturePro(iconsTexture, m_icons[i].sourceRect, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
            }
        } else if (i < ICONS_PER_ROW * 2) {
            destination.x = OFFSET_X  + ( GAP_X * (i - ICONS_PER_ROW));
            destination.y = 480 - 80 + 8;

            if (
                m_selectedId == static_cast<int>(i)
                || m_hoverId == static_cast<int>(i)
            ) {
                DrawRectangle(destination.x - 8, destination.y - 8, destination.width + 16, destination.height + 16, bgColour);
            }

            if (m_icons[i].actionType == ICON_ACTION_TYPE::STATS) {
                DrawPetAtSpot(destination);
            } else {
                DrawTexturePro(iconsTexture, m_icons[i].sourceRect, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
            }
        } else {
            // nothing?
        }
    }
}

bool TamaUI::IsUIShown() {
    return !this->hideUI;
}

void TamaUI::DrawPetAtSpot(rlRectangle destination) {
    Pet& petData = m_game->m_gameData.GetCurrentPet();
    Texture& petTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentPetTexturePath());
    Texture& faceTexture = m_game->m_resourceManager.GetTexture("textures/faces.png");

    if (petData.stage == PET_STAGES::EGG) {
        DrawTexturePro(
            petTexture,
            { 0, 0, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            destination,
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
        return;
    }

    if (petData.state == PET_STATES::DED) {
        DrawTexturePro(
            petTexture,
            { 0, 0, m_game->m_gameData.GetCurrentPetWidth(), -m_game->m_gameData.GetCurrentPetHeight() },
            destination,
            { 0.0f, 0.0f },
            0.0f,
            m_game->m_gameData.PetTintList[petData.petTint]
        );
    } else {
        DrawTexturePro(
            petTexture,
            { m_game->m_gameData.GetCurrentPetWidth(), 0, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            destination,
            { 0.0f, 0.0f },
            0.0f,
            m_game->m_gameData.PetTintList[petData.petTint]
        );
    }

    if (petData.stage == PET_STAGES::EGG) {
        return;
    }

    if (petData.state == PET_STATES::DED) {
        DrawTexturePro(
            faceTexture,
            { m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y, m_game->m_gameData.GetCurrentFace().width, -m_game->m_gameData.GetCurrentFace().height},
            destination,
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    } else {
        DrawTexturePro(
            faceTexture,
            { m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y, m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
            destination,
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }

}
