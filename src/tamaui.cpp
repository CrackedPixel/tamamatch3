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

    int promptsCount = m_game->m_gameData.INICount("prompts");
    for (int i = 0; i < promptsCount; ++i) {
        const char* promptKey = m_game->m_gameData.INIString("prompts", std::to_string(i+1).c_str(), "");
        std::string promptMessage = Utils::ReplaceNewlines(m_game->m_gameData.INIString("prompts", promptKey, "MISSING"));
        m_promptsList.emplace(promptKey, promptMessage);
    }

    m_barsColour = Utils::RGBAFromString(m_game->m_gameData.INIString("colours", "bars", "182 182 182 255"));
}

void TamaUI::OnTerminate() {
    m_icons.clear();
}

void TamaUI::OnUpdate(float deltaTime) {
    if (m_popupMenu == POPUP_TYPES::NONE && m_game->m_inputController.IsButtonUp) {
        m_selectedId -= 1;

        if (m_selectedId < 0) {
            m_selectedId = m_icons.size() - 1;
        }

        return;
    }

    if (m_popupMenu == POPUP_TYPES::NONE && m_game->m_inputController.IsButtonDown) {
        m_selectedId += 1;

        if (m_selectedId == static_cast<int>(m_icons.size())) {
            m_selectedId = 0;
        }

        return;
    }

    if (m_popupMenu == POPUP_TYPES::NONE && m_game->m_inputController.IsButtonBack) {
        if (m_hideUI) {
            m_hideUI = false;
            return;
        }
    }

    if (m_popupMenu == POPUP_TYPES::INVENTORY) {
        if (m_game->m_inputController.IsButtonUp) {
            switch (m_currentInventoryTab) {
                case 0: { // top
                    m_currentInventoryPage = static_cast<INVENTORY_PAGES>(static_cast<int>(m_currentInventoryPage)-1);
                    if (m_currentInventoryPage < INVENTORY_PAGES::FOOD) {
                        m_currentInventoryPage = INVENTORY_PAGES::ACC2;
                    }
                    m_currentInventorySlot = 0;
                } break;
                case 1: { // bottom
                    m_currentInventorySlot--;
                } break;
            }
        }

        if (m_game->m_inputController.IsButtonDown) {
            switch (m_currentInventoryTab) {
            case 0: { // top
                m_currentInventoryPage = static_cast<INVENTORY_PAGES>(static_cast<int>(m_currentInventoryPage)+1);
                if (m_currentInventoryPage == INVENTORY_PAGES::COUNT) {
                    m_currentInventoryPage = INVENTORY_PAGES::FOOD;
                }
                m_currentInventorySlot = 0;
            } break;
            case 1: { // bottom
                m_currentInventorySlot++;
            } break;
            }
        }

        if (m_game->m_inputController.IsButtonSelect) {
            switch (m_currentInventoryTab) {
                case 0: { // top
                    m_currentInventoryTab = 1;
                    return;
                } break;
                case 1: { // bottom, item selection
                    switch (m_currentInventoryPage) {
                        case INVENTORY_PAGES::FOOD: {
                            if (m_game->m_gameData.GetCurrentPetFoodCount(static_cast<FOOD_TYPES>(m_currentInventorySlot)) > 0) {
                                m_game->m_gameData.activeCursor = static_cast<CURSOR_TYPES>(static_cast<int>(CURSOR_TYPES::FOOD1) + m_currentInventorySlot);
                                m_popupMenu = POPUP_TYPES::NONE;
                            }
                        } break;
                        case INVENTORY_PAGES::HATS: {
                            const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::HAT;
                            auto& currentPet = m_game->m_gameData.GetCurrentPet();

                            if (m_currentInventorySlot == 0) {
                                currentPet.outfitId[viewSlot] = 0;
                                return;
                            }

                            auto& selectedItem = currentPet.outfitInventory[viewSlot][m_currentInventorySlot-1];
                            currentPet.outfitId[viewSlot] = selectedItem.outfitId;
                            currentPet.outfitTint[viewSlot] = selectedItem.outfitTint;
                        } break;
                        case INVENTORY_PAGES::ACC1: {
                            const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::ACC1;
                            auto& currentPet = m_game->m_gameData.GetCurrentPet();

                            if (m_currentInventorySlot == 0) {
                                currentPet.outfitId[viewSlot] = 0;
                                return;
                            }

                            auto& selectedItem = currentPet.outfitInventory[viewSlot][m_currentInventorySlot-1];
                            currentPet.outfitId[viewSlot] = selectedItem.outfitId;
                            currentPet.outfitTint[viewSlot] = selectedItem.outfitTint;
                        } break;
                        case INVENTORY_PAGES::ACC2: {
                            const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::ACC2;
                            auto& currentPet = m_game->m_gameData.GetCurrentPet();

                            if (m_currentInventorySlot == 0) {
                                currentPet.outfitId[viewSlot] = 0;
                                return;
                            }

                            auto& selectedItem = currentPet.outfitInventory[viewSlot][m_currentInventorySlot-1];
                            currentPet.outfitId[viewSlot] = selectedItem.outfitId;
                            currentPet.outfitTint[viewSlot] = selectedItem.outfitTint;
                        } break;
                        case INVENTORY_PAGES::GLASSES: {
                            const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::GLASSES;
                            auto& currentPet = m_game->m_gameData.GetCurrentPet();

                            if (m_currentInventorySlot == 0) {
                                currentPet.outfitId[viewSlot] = 0;
                                return;
                            }

                            auto& selectedItem = currentPet.outfitInventory[viewSlot][m_currentInventorySlot-1];
                            currentPet.outfitId[viewSlot] = selectedItem.outfitId;
                            currentPet.outfitTint[viewSlot] = selectedItem.outfitTint;
                        } break;
                        case INVENTORY_PAGES::BACK: {
                            const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::BACK;
                            auto& currentPet = m_game->m_gameData.GetCurrentPet();

                            if (m_currentInventorySlot == 0) {
                                currentPet.outfitId[viewSlot] = 0;
                                return;
                            }

                            auto& selectedItem = currentPet.outfitInventory[viewSlot][m_currentInventorySlot-1];
                            currentPet.outfitId[viewSlot] = selectedItem.outfitId;
                            currentPet.outfitTint[viewSlot] = selectedItem.outfitTint;
                        } break;
                    }
                    return;
                };
            }
        }

        if (m_game->m_inputController.IsButtonBack) {
            switch (m_currentInventoryTab) {
                case 0: { // top
                    m_popupMenu = POPUP_TYPES::NONE;
                    return;
                } break;
                case 1: { // bottom, item selection
                    m_currentInventoryTab = 0;
                    return;
                };
            }
        }
    }

    if (m_game->m_inputController.IsButtonSelect) {
        switch (m_popupMenu) {
            case POPUP_TYPES::STATS: {
                m_popupMenu = POPUP_TYPES::NONE;
                return;
            } break;
            case POPUP_TYPES::PAUSE_MENU: {

            } break;
            case POPUP_TYPES::EVOLVE: {
                m_game->m_gameData.EvolveCurrentPet();
                m_game->m_audioManager->PlaySFX("hatch");
                m_popupMenu = POPUP_TYPES::NONE;

                return;
            } break;
            default: break;
        }

        if (m_hideUI) {
            m_hideUI = false;
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

        // petData.attributes[PET_ATTRIBUTES::HUNGER] += GetRandomValue(10, 20) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::HAPPINESS] -= GetRandomValue(10, 26) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::BOREDOM] += GetRandomValue(15, 40) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::HYGIENE] -= GetRandomValue(15, 50) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::TANKHYGIENE] -= GetRandomValue(10, 26) * multiplySpeed;
        // petData.attributes[PET_ATTRIBUTES::ILLNESS] -= GetRandomValue(5, 15) * multiplySpeed;


        // device select
        switch (m_icons[m_selectedId].actionType) {
            case ICON_ACTION_TYPE::STATS: {
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                m_popupMenu = POPUP_TYPES::STATS;
                m_game->m_audioManager->PlaySFX("uiselect");
            } break;
            case ICON_ACTION_TYPE::BANDAID: {
                // m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                auto& petData = m_game->m_gameData.GetCurrentPet();
                petData.attributes[PET_ATTRIBUTES::ILLNESS] += 0.3f;
                m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::ILLNESS, { m_sceneTama->GetPetAI()->GetPetPosition().x, m_sceneTama->GetPetAI()->GetPetPosition().y });
                m_game->m_audioManager->PlaySFX("heal");
            } break;
            case ICON_ACTION_TYPE::TOY: {
                // m_game->m_gameData.activeCursor = CURSOR_TYPES::TOY;
                auto& petData = m_game->m_gameData.GetCurrentPet();
                petData.attributes[PET_ATTRIBUTES::BOREDOM] -= 0.5f;
                petData.attributes[PET_ATTRIBUTES::HAPPINESS] += 0.35f;
                // Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::BOREDOM]);
                // Utils::ClampRange(petData.attributes[PET_ATTRIBUTES::HAPPINESS]);
                m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::TOY, { m_sceneTama->GetPetAI()->GetPetPosition().x, m_sceneTama->GetPetAI()->GetPetPosition().y });
                m_game->m_audioManager->PlaySFX("boop", 1, 2);
            } break;
            case ICON_ACTION_TYPE::CLEAN: {
                // m_game->m_gameData.activeCursor = CURSOR_TYPES::DIRTY;
                auto& petData = m_game->m_gameData.GetCurrentPet();
                petData.attributes[PET_ATTRIBUTES::HYGIENE] += 0.5f;
                m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::DIRTY, { m_sceneTama->GetPetAI()->GetPetPosition().x, m_sceneTama->GetPetAI()->GetPetPosition().y });
                m_game->m_audioManager->PlaySFX("shower");
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
                    m_game->m_audioManager->PlaySFX("flush");
                }
            } break;

            case ICON_ACTION_TYPE::CAMERA: {
                m_hideUI = !m_hideUI;
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                m_game->m_audioManager->PlaySFX("uiselect");
            } break;
            case ICON_ACTION_TYPE::INVENTORY: {
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                m_currentInventoryPage = INVENTORY_PAGES::FOOD;
                m_currentInventorySlot = 0;
                m_currentInventoryTab = 0;
                m_popupMenu = POPUP_TYPES::INVENTORY;
                m_game->m_audioManager->PlaySFX("uiselect");
            } break;
            case ICON_ACTION_TYPE::MINIGAMES: {
                // TODO: change scene
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                m_game->m_audioManager->PlaySFX("uiselect");
            } break;
            case ICON_ACTION_TYPE::STORE: {
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                m_game->m_audioManager->PlaySFX("uiselect");
                m_game->ChangeScene("shop", true, 0.5f);
            } break;
            case ICON_ACTION_TYPE::DISPLAY: {
                m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                m_game->m_audioManager->PlaySFX("uiselect");
                m_game->ChangeScene("collection", true, 0.5f);
            } break;

        }

        // m_game->m_audioManager->PlaySFX("uiselect");

        return;
    }

    if (m_popupMenu != POPUP_TYPES::NONE) {
        m_hoverId = -1;
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

    if (m_popupMenu != POPUP_TYPES::NONE) {
        return false;
    }

    if (m_hoverId == -1) {
        Vector2 mousePosition = GetMousePosition();
        mousePosition.x -= 184;
        mousePosition.y -= 174;

        if (CheckCollisionPointRec(mousePosition, petPosition)) {
            Pet& petData = m_game->m_gameData.GetCurrentPet();
            switch (m_game->m_gameData.activeCursor) {
                case CURSOR_TYPES::DIRTY: {
                    petData.attributes[PET_ATTRIBUTES::HYGIENE] += 0.45f;
                    m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::DIRTY, mousePosition);
                    m_game->m_audioManager->PlaySFX("shower");
                } break;
                case CURSOR_TYPES::ILLNESS: {
                    petData.attributes[PET_ATTRIBUTES::ILLNESS] += 0.45f;
                    m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::ILLNESS, mousePosition);
                    m_game->m_audioManager->PlaySFX("heal");
                } break;
                case CURSOR_TYPES::TOY: {
                    petData.attributes[PET_ATTRIBUTES::BOREDOM] -= 0.45f;
                    m_sceneTama->GetPetAI()->SpawnNewInteractSpot(CURSOR_TYPES::TOY, mousePosition);
                    m_game->m_audioManager->PlaySFX("boop", 1, 2);
                } break;

                case CURSOR_TYPES::FOOD1:
                case CURSOR_TYPES::FOOD2:
                case CURSOR_TYPES::FOOD3:
                case CURSOR_TYPES::FOOD4:
                case CURSOR_TYPES::FOOD5: {
                    petData.attributes[PET_ATTRIBUTES::HUNGER] -= 0.45f;
                    m_sceneTama->GetPetAI()->SpawnNewFoodSpot(m_game->m_gameData.activeCursor, mousePosition);
                    m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
                    m_game->m_audioManager->PlaySFX("nom", 1, 2);
                    return true;
                }
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
                    m_game->m_audioManager->PlaySFX("flush");
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
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            m_popupMenu = POPUP_TYPES::STATS;
            m_game->m_audioManager->PlaySFX("uiselect");
        } break;
        case ICON_ACTION_TYPE::BANDAID: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::ILLNESS;
            // m_game->m_audioManager->PlaySFX("heal");
        } break;
        case ICON_ACTION_TYPE::TOY: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::TOY;
            // m_game->m_audioManager->PlaySFX("boop", 1, 2);
        } break;
        case ICON_ACTION_TYPE::CLEAN: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::DIRTY;
            // m_game->m_audioManager->PlaySFX("shower");
        } break;
        case ICON_ACTION_TYPE::CLEAN_TANK: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::TANKDIRTY;
            // m_game->m_audioManager->PlaySFX("flush");
        } break;

        case ICON_ACTION_TYPE::CAMERA: {
            m_hideUI = !m_hideUI;

            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            m_game->m_audioManager->PlaySFX("uiselect");
        } break;
        case ICON_ACTION_TYPE::INVENTORY: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            m_currentInventoryPage = INVENTORY_PAGES::FOOD;
            m_currentInventorySlot = 0;
            m_currentInventoryTab = 0;
            m_popupMenu = POPUP_TYPES::INVENTORY;
            m_game->m_audioManager->PlaySFX("uiselect");
        } break;
        case ICON_ACTION_TYPE::MINIGAMES: {
            // TODO: change scene
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            m_game->m_audioManager->PlaySFX("uiselect");
        } break;
        case ICON_ACTION_TYPE::STORE: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            m_game->m_audioManager->PlaySFX("uiselect");
            m_game->ChangeScene("shop", true, 0.5f);
        } break;
        case ICON_ACTION_TYPE::DISPLAY: {
            m_selectedId = m_hoverId;
            m_game->m_gameData.activeCursor = CURSOR_TYPES::NORMAL;
            m_game->m_audioManager->PlaySFX("uiselect");
            m_game->ChangeScene("collection", true, 0.5f);
        } break;
    }

    // m_game->m_audioManager->PlaySFX("uiselect");

    return true;
}

void TamaUI::OnRenderUI() {
    Texture& popupTexture = m_game->m_resourceManager.GetTexture("textures/messagebox.png", 0);
    switch (m_popupMenu) {
    case POPUP_TYPES::STATS: {
        DrawRectangleRounded({ 140, 130, 380, 230 }, 0.2f, 1, { 111, 180, 68, 255 });
        DrawRectangleRoundedLinesEx({ 140, 130, 380, 230 }, 0.2f, 1, 3, BLACK);

        // DrawRectangleRounded({ 160, 150, 100, 100 }, 0.2f, 1, YELLOW);
        // DrawPetAtSpot({ 170, 160, 80, 80 });
        auto& petData = m_game->m_gameData.GetCurrentPet();
        if (petData.stage != PET_STAGES::ADULT) {
            rlDrawText(TextFormat("Growth: %d\%\nHP: %d\%\nHunger: %d\%\nBoredom: %d\%",
              static_cast<int>(100.0f * (petData.attributes[PET_ATTRIBUTES::GROWTH] / m_game->m_gameData.GetCurrentPetGrowthTime())),
              static_cast<int>(100.0f * petData.attributes[PET_ATTRIBUTES::HP]),
              static_cast<int>(100.0f * petData.attributes[PET_ATTRIBUTES::HUNGER]),
              static_cast<int>(100.0f * petData.attributes[PET_ATTRIBUTES::BOREDOM])
            ), 160, 150, 22, BLACK);

            rlDrawText(TextFormat("Sadness: %d\%\nDirty: %d\%\nTank: %d\%\nIllness: %d\%",
              static_cast<int>(100.0f * (1-petData.attributes[PET_ATTRIBUTES::HAPPINESS])),
              static_cast<int>(100.0f * (1-petData.attributes[PET_ATTRIBUTES::HYGIENE])),
              static_cast<int>(100.0f * (1-petData.attributes[PET_ATTRIBUTES::TANKHYGIENE])),
              static_cast<int>(100.0f * (1-petData.attributes[PET_ATTRIBUTES::ILLNESS]))
            ), 350, 150, 22, BLACK);
        } else {
            rlDrawText("Growth: 100%\nHP: 100%\nHunger: 0%\nBoredom: 0%", 160, 150, 22, BLACK);
            rlDrawText("Sadness: 0%\nDirty: 0%\nTank: 0%\nIllness: 0%", 350, 150, 22, BLACK);
        }
    } break;
    case POPUP_TYPES::PAUSE_MENU: {

    } break;
    case POPUP_TYPES::INVENTORY: {
        DrawTexturePro(popupTexture, { 0, 0, 128, 128 }, { 192, 112, 256, 256 }, { 0, 0 }, 0.0f, WHITE);

        Color drawColour;
        drawColour = m_currentInventoryTab == 0 ? BLACK : GRAY;
        rlDrawText(m_inventoryPageTitles[static_cast<int>(m_currentInventoryPage)], 192 + (128 - (MeasureText(m_inventoryPageTitles[static_cast<int>(m_currentInventoryPage)], 30) * 0.5f)), 140, 30, drawColour);
        rlDrawText("<", 192 + 20, 140, 30, drawColour);
        rlDrawText(">", 192 + 256 - 30, 140, 30, drawColour);

        drawColour = m_currentInventoryTab == 1 ? BLACK : GRAY;
        rlDrawText("<", 192 + 20, 250, 30, drawColour);
        rlDrawText(">", 192 + 256 - 30, 250, 30, drawColour);
        //  { "Food", "Hats", "Glasses", "Backs", "Acc" };
        switch (m_currentInventoryPage) {
            case INVENTORY_PAGES::FOOD: {
                if (m_currentInventorySlot >= static_cast<int>(FOOD_TYPES::COUNT)) {
                    m_currentInventorySlot = 0;
                }
                if (m_currentInventorySlot < 0) {
                    m_currentInventorySlot = static_cast<int>(FOOD_TYPES::COUNT)-1;
                }
                rlDrawText(TextFormat("x%d", m_game->m_gameData.GetCurrentPetFoodCount(static_cast<FOOD_TYPES>(m_currentInventorySlot))), 320 - (MeasureText(TextFormat("x%d", m_game->m_gameData.GetCurrentPetFoodCount(static_cast<FOOD_TYPES>(m_currentInventorySlot))), 30) * 0.5f), 128+256-60, 30, drawColour);
                Texture& foodTexture = m_game->m_resourceManager.GetTexture("textures/food.png", 0);
                drawColour = m_currentInventoryTab == 1 ? WHITE : GRAY;
                DrawTexturePro(foodTexture, m_game->m_gameData.FoodList[static_cast<FOOD_TYPES>(m_currentInventorySlot)], { 256, 200, 128, 128 }, { 0, 0 }, 0.0f, drawColour);
            } break;
            case INVENTORY_PAGES::HATS: {
                const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::HAT;
                auto& currentPet = m_game->m_gameData.GetCurrentPet();
                if (static_cast<int>(currentPet.outfitInventory[viewSlot].size() == 0)) {
                    m_currentInventorySlot = 0;
                } else {
                    if (m_currentInventorySlot >= static_cast<int>(currentPet.outfitInventory[viewSlot].size())+1) {
                        m_currentInventorySlot = 0;
                    }
                    if (m_currentInventorySlot < 0) {
                        m_currentInventorySlot = static_cast<int>(currentPet.outfitInventory[viewSlot].size());
                    }
                }

                if (m_currentInventorySlot == 0) {
                    rlDrawText("None", 320 - (MeasureText("None", 30) * 0.5f), 250, 30, drawColour);
                } else {
                    auto& selectedItem = currentPet.outfitInventory[viewSlot][m_currentInventorySlot-1];

                    auto& outfitData = m_game->m_gameData.OutfitList[viewSlot][selectedItem.outfitId - 1];
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath, 0);
                    if (outfitData.isColourable) {
                        drawColour = m_game->m_gameData.OutfitTintList[selectedItem.outfitTint - 1];
                    } else {
                        drawColour = WHITE;
                    }
                    drawColour = m_currentInventoryTab == 1 ? drawColour : ColorTint(drawColour, GRAY);
                    DrawTexturePro(outfitTexture, outfitData.texturePosition, { 256, 200, 128, 128 }, { 0, 0 }, 0.0f, drawColour);
                }
            } break;
            case INVENTORY_PAGES::GLASSES: {
                const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::GLASSES;
                auto& currentPet = m_game->m_gameData.GetCurrentPet();
                if (static_cast<int>(currentPet.outfitInventory[viewSlot].size() == 0)) {
                    m_currentInventorySlot = 0;
                } else {
                    if (m_currentInventorySlot >= static_cast<int>(currentPet.outfitInventory[viewSlot].size())+1) {
                        m_currentInventorySlot = 0;
                    }
                    if (m_currentInventorySlot < 0) {
                        m_currentInventorySlot = static_cast<int>(currentPet.outfitInventory[viewSlot].size());
                    }
                }

                if (m_currentInventorySlot == 0) {
                    rlDrawText("None", 320 - (MeasureText("None", 30) * 0.5f), 250, 30, drawColour);
                } else {
                    auto& selectedItem = currentPet.outfitInventory[viewSlot][m_currentInventorySlot-1];

                    auto& outfitData = m_game->m_gameData.OutfitList[viewSlot][selectedItem.outfitId - 1];
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath, 0);
                    if (outfitData.isColourable) {
                        drawColour = m_game->m_gameData.OutfitTintList[selectedItem.outfitTint - 1];
                    } else {
                        drawColour = WHITE;
                    }
                    drawColour = m_currentInventoryTab == 1 ? drawColour : ColorTint(drawColour, GRAY);
                    DrawTexturePro(outfitTexture, outfitData.texturePosition, { 256, 200, 128, 128 }, { 0, 0 }, 0.0f, drawColour);
                }
            } break;
            case INVENTORY_PAGES::BACK: {
                const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::BACK;
                auto& currentPet = m_game->m_gameData.GetCurrentPet();
                if (static_cast<int>(currentPet.outfitInventory[viewSlot].size() == 0)) {
                    m_currentInventorySlot = 0;
                } else {
                    if (m_currentInventorySlot >= static_cast<int>(currentPet.outfitInventory[viewSlot].size())+1) {
                        m_currentInventorySlot = 0;
                    }
                    if (m_currentInventorySlot < 0) {
                        m_currentInventorySlot = static_cast<int>(currentPet.outfitInventory[viewSlot].size());
                    }
                }

                if (m_currentInventorySlot == 0) {
                    rlDrawText("None", 320 - (MeasureText("None", 30) * 0.5f), 250, 30, drawColour);
                } else {
                    auto& selectedItem = currentPet.outfitInventory[viewSlot][m_currentInventorySlot-1];

                    auto& outfitData = m_game->m_gameData.OutfitList[viewSlot][selectedItem.outfitId - 1];
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath, 0);
                    if (outfitData.isColourable) {
                        drawColour = m_game->m_gameData.OutfitTintList[selectedItem.outfitTint - 1];
                    } else {
                        drawColour = WHITE;
                    }
                    drawColour = m_currentInventoryTab == 1 ? drawColour : ColorTint(drawColour, GRAY);
                    DrawTexturePro(outfitTexture, outfitData.texturePosition, { 256, 200, 128, 128 }, { 0, 0 }, 0.0f, drawColour);
                }
            } break;
            case INVENTORY_PAGES::ACC1: {
                const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::ACC1;
                auto& currentPet = m_game->m_gameData.GetCurrentPet();
                if (static_cast<int>(currentPet.outfitInventory[viewSlot].size() == 0)) {
                    m_currentInventorySlot = 0;
                } else {
                    if (m_currentInventorySlot >= static_cast<int>(currentPet.outfitInventory[viewSlot].size())+1) {
                        m_currentInventorySlot = 0;
                    }
                    if (m_currentInventorySlot < 0) {
                        m_currentInventorySlot = static_cast<int>(currentPet.outfitInventory[viewSlot].size());
                    }
                }

                if (m_currentInventorySlot == 0) {
                    rlDrawText("None", 320 - (MeasureText("None", 30) * 0.5f), 250, 30, drawColour);
                } else {
                    auto& selectedItem = currentPet.outfitInventory[OUTFIT_SLOTS::ACC1][m_currentInventorySlot-1];

                    auto& outfitData = m_game->m_gameData.OutfitList[viewSlot][selectedItem.outfitId - 1];
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath, 0);
                    if (outfitData.isColourable) {
                        drawColour = m_game->m_gameData.OutfitTintList[selectedItem.outfitTint - 1];
                    } else {
                        drawColour = WHITE;
                    }
                    drawColour = m_currentInventoryTab == 1 ? drawColour : ColorTint(drawColour, GRAY);
                    DrawTexturePro(outfitTexture, outfitData.texturePosition, { 256, 200, 128, 128 }, { 0, 0 }, 0.0f, drawColour);
                }
            } break;
            case INVENTORY_PAGES::ACC2: {
                const OUTFIT_SLOTS viewSlot = OUTFIT_SLOTS::ACC2;
                auto& currentPet = m_game->m_gameData.GetCurrentPet();
                if (static_cast<int>(currentPet.outfitInventory[viewSlot].size() == 0)) {
                    m_currentInventorySlot = 0;
                } else {
                    if (m_currentInventorySlot >= static_cast<int>(currentPet.outfitInventory[viewSlot].size())+1) {
                        m_currentInventorySlot = 0;
                    }
                    if (m_currentInventorySlot < 0) {
                        m_currentInventorySlot = static_cast<int>(currentPet.outfitInventory[viewSlot].size());
                    }
                }

                if (m_currentInventorySlot == 0) {
                    rlDrawText("None", 320 - (MeasureText("None", 30) * 0.5f), 250, 30, drawColour);
                } else {
                    auto& selectedItem = currentPet.outfitInventory[viewSlot][m_currentInventorySlot-1];

                    auto& outfitData = m_game->m_gameData.OutfitList[OUTFIT_SLOTS::ACC1][selectedItem.outfitId - 1];
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath, 0);
                    if (outfitData.isColourable) {
                        drawColour = m_game->m_gameData.OutfitTintList[selectedItem.outfitTint - 1];
                    } else {
                        drawColour = WHITE;
                    }
                    drawColour = m_currentInventoryTab == 1 ? drawColour : ColorTint(drawColour, GRAY);
                    DrawTexturePro(outfitTexture, outfitData.texturePosition, { 256, 200, 128, 128 }, { 0, 0 }, 0.0f, drawColour);
                }
            } break;
            default: break;
        }
    } break;
    case POPUP_TYPES::EVOLVE: {
        DrawTexturePro(popupTexture, { 0, 0, 128, 128 }, { 192, 112, 256, 256 }, { 0, 0 }, 0.0f, WHITE);
        rlDrawText("Growth", 220, 140, 30, BLACK);
        rlDrawText(m_promptsList["evolve_egg"].c_str(), 220, 200, 20, BLACK);
    } break;
    default: break;
    }

    if (m_hideUI) {
        return;
    }

    Texture& iconsTexture = m_game->m_resourceManager.GetTexture(ICONS_TEXTURE_PATH, 0);

    Color bgColour;
    rlRectangle destination = { 0.0f, 0.0f, 64, 64 };

    DrawRectangle(0, 0, 640, 80, m_barsColour);
    DrawRectangle(0, 480 - 80, 640, 80, m_barsColour);

    for (size_t i = 0; i < m_icons.size(); ++i) {
        bgColour = m_hoverId == static_cast<int>(i) ? Color{ 172, 211, 148, 255 } : Color{ 111, 180, 68, 255 };
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
    return !this->m_hideUI;
}

void TamaUI::DrawPetAtSpot(rlRectangle destination) {
    Pet& petData = m_game->m_gameData.GetCurrentPet();
    Texture& petTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentPetTexturePath());
    Texture& faceTexture = m_game->m_resourceManager.GetTexture("textures/faces.png", 0);

    float widthFactor = m_game->m_gameData.GetCurrentPetWidth() / 64;
    float widthHeightFactor = m_game->m_gameData.GetCurrentPetWidth() / m_game->m_gameData.GetCurrentPetHeight();

    switch (petData.stage) {
        case PET_STAGES::EGG: {
            DrawTexturePro(
                petTexture,
                { 0, 0, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
                destination,
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
            return;
        } break;
        case PET_STAGES::NEWBORN: {
            DrawTexturePro(
                petTexture,
                { m_game->m_gameData.GetCurrentPetWidth(), 0, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
                destination,
                { 0.0f, 0.0f },
                0.0f,
                m_game->m_gameData.PetTintList[petData.petTint]
            );
        } break;
        case PET_STAGES::TODDLER: {
            DrawTexturePro(
                petTexture,
                { m_game->m_gameData.GetCurrentPetWidth(), 0, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
                { destination.x, destination.y + 16, destination.width, destination.height / widthHeightFactor },
                { 0.0f, 0.0f },
                0.0f,
                m_game->m_gameData.PetTintList[petData.petTint]
            );
        } break;
        case PET_STAGES::ADOLESCENT: {
            DrawTexturePro(
                petTexture,
                { m_game->m_gameData.GetCurrentPetWidth(), 0, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
                { destination.x, destination.y + 16, destination.width, destination.height / widthHeightFactor },
                { 0.0f, 0.0f },
                0.0f,
                m_game->m_gameData.PetTintList[petData.petTint]
            );
        } break;
        case PET_STAGES::ADULT: {
            DrawTexturePro(
                petTexture,
                { 0, 0, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
                destination,
                { 0.0f, 0.0f },
                0.0f,
                m_game->m_gameData.PetTintList[petData.petTint]
            );
        } break;
    }

    // FACES
    switch (petData.stage) {
        case PET_STAGES::EGG: {
            return;
        } break;
        case PET_STAGES::NEWBORN: {
            DrawTexturePro(
                faceTexture,
                { m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y, m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
                { destination.x, destination.y + ((widthFactor-1)*10), destination.width / widthFactor, destination.height / widthHeightFactor },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        } break;
        case PET_STAGES::TODDLER: {
            DrawTexturePro(
                faceTexture,
                { m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y, m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
                { destination.x, destination.y + ((widthFactor-1)*10), destination.width / widthFactor, destination.height / widthHeightFactor },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        } break;
        case PET_STAGES::ADOLESCENT: {
            DrawTexturePro(
                faceTexture,
                { m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y, m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
                { destination.x, destination.y + ((widthFactor-1)*10), destination.width / widthFactor, destination.height / widthHeightFactor },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        } break;
        case PET_STAGES::ADULT: {
            DrawTexturePro(
                faceTexture,
                { m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y, m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
                { destination.x + 17, destination.y + 16, destination.width / 2, destination.height / 2 },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        } break;
    }
}
