#include "scene_shop.hpp"

#include "game.hpp"

void SceneShop::OnInitialize() {
    m_shopState = SHOP_STATES::NEW;
    m_game->m_transitionManager.FadeIn(0.5f);

    m_itemPrice = m_game->m_gameData.INIInt("shop", "item_price", 10);
}

void SceneShop::OnUpdate(float deltaTime) {
    if (m_game->IsChangingScenes()) {
        return;
    }

    // TEMP: animate?
    if (m_shopState == SHOP_STATES::BUYING) {
        m_game->m_gameData.GetCurrentPet().AddNewInventoryItem(m_wonItem);
        m_shopState = SHOP_STATES::BUY_REVIEW;
        return;
    }

    if (m_game->m_inputController.IsButtonBack) {
        switch (m_shopState) {
        case SHOP_STATES::NEW: {
            m_game->ChangeScene("tama", true, 0.5f);
            return;
        } break;
        case SHOP_STATES::BUYING: {

        } break;
        case SHOP_STATES::BUY_REVIEW: {
            m_shopState = SHOP_STATES::NEW;
            return;
        } break;
        default: break;
        }
    }

    if (m_game->m_inputController.IsButtonSelect) {
        switch (m_shopState) {
            case SHOP_STATES::BROKE: {
                m_shopState = SHOP_STATES::NEW;
            } break;
            case SHOP_STATES::NEW: {
                if (m_game->m_gameData.coins < m_itemPrice) {
                    m_shopState = SHOP_STATES::BROKE;
                    return;
                }

                m_game->m_gameData.coins -= m_itemPrice;

                if (GetRandomValue(0, 3) == 0) {
                    m_wonItem.itemType = ITEM_TYPES::OUTFIT;

                    m_wonItem.slot = GetRandomValue(1, 4);
                    switch (m_wonItem.slot) {
                        case 1: {
                            m_wonItem.slot = static_cast<int>(OUTFIT_SLOTS::HAT);
                        } break;
                        case 2: {
                            m_wonItem.slot = static_cast<int>(OUTFIT_SLOTS::GLASSES);
                        } break;
                        case 3: {
                            m_wonItem.slot = static_cast<int>(OUTFIT_SLOTS::BACK);
                        } break;
                        case 4: {
                            m_wonItem.slot = static_cast<int>(OUTFIT_SLOTS::ACC1);
                        } break;
                    }

                    m_wonItem.id = GetRandomValue(1, m_game->m_gameData.OutfitList[static_cast<OUTFIT_SLOTS>(m_wonItem.slot)].size());

                    if (m_game->m_gameData.OutfitList[static_cast<OUTFIT_SLOTS>(m_wonItem.slot)][m_wonItem.id-1].isColourable) {
                        m_wonItem.tint = GetRandomValue(1, m_game->m_gameData.OutfitTintList.size());
                    } else {
                        m_wonItem.tint = 0;
                    }
                } else {
                    m_wonItem.itemType = ITEM_TYPES::FOOD;

                    m_wonItem.id = GetRandomValue(1, static_cast<int>(FOOD_TYPES::COUNT));
                }
                m_shopState = SHOP_STATES::BUYING;
            } break;
            case SHOP_STATES::BUYING: {

            } break;
            case SHOP_STATES::BUY_REVIEW: {
                m_shopState = SHOP_STATES::NEW;
                return;
            } break;
            default: break;
        }
    }
}

void SceneShop::OnRender() {
    Texture& wallpaperTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentWallpaper());
    Texture& capsuleTexture = m_game->m_resourceManager.GetTexture("textures/capsulemachine.png");

    DrawTexture(wallpaperTexture, 0, 0, WHITE);
    DrawTexture(capsuleTexture, 0, 0, WHITE);
}

void SceneShop::OnRenderUI() {
    Texture& miscTexture = m_game->m_resourceManager.GetTexture("textures/misc.png");
    Texture& cursorTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentCursorPath(), 0);

    DrawTexturePro(miscTexture, { 0, 32, 64, 64 }, { 20, 10, 64, 64 }, { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(miscTexture, { 64, 0, 64, 64 }, { 20, 80, 64, 64 }, { 0, 0 }, 0.0f, WHITE);

    rlDrawText(TextFormat("%d", m_game->m_gameData.coins), 90, 32, 25, BLACK);
    rlDrawText(TextFormat("%d", m_itemPrice), 90, 102, 25, BLACK);

    switch (m_shopState) {
        case SHOP_STATES::NEW: {

        } break;
        case SHOP_STATES::BROKE: {
            DrawRectangleRounded({ 320 - (380 * 0.5f), 130, 380, 230 }, 0.2f, 1, { 111, 180, 68, 255 });
            DrawRectangleRoundedLinesEx({ 320 - (380 * 0.5f), 130, 380, 230 }, 0.2f, 1, 3, BLACK);
            rlDrawText("Insufficient coins", 320 - (MeasureText("Insufficient coins", 40) * 0.5f), 150, 40, BLACK);
        } break;
        case SHOP_STATES::BUYING: {

        } break;
        case SHOP_STATES::BUY_REVIEW: {
            DrawRectangleRounded({ 320 - (380 * 0.5f), 130, 380, 230 }, 0.2f, 1, { 111, 180, 68, 255 });
            DrawRectangleRoundedLinesEx({ 320 - (380 * 0.5f), 130, 380, 230 }, 0.2f, 1, 3, BLACK);
            rlDrawText("You won...", 320 - (MeasureText("You won...", 20) * 0.5f), 150, 20, BLACK);

            switch (m_wonItem.itemType) {
                case ITEM_TYPES::FOOD: {
                    Texture& foodTexture = m_game->m_resourceManager.GetTexture("textures/food.png", 0);
                    DrawTexturePro(foodTexture, m_game->m_gameData.FoodList[static_cast<FOOD_TYPES>(m_wonItem.id-1)], { 256, 190, 128, 128 }, { 0, 0 }, 0.0f, WHITE);
                    rlDrawText("Food!", 320 - (MeasureText("Food!", 30) * 0.5f), 320, 30, BLACK);
                } break;
                case ITEM_TYPES::OUTFIT: {
                    const auto& outfitData = m_game->m_gameData.OutfitList[static_cast<OUTFIT_SLOTS>(m_wonItem.slot)][m_wonItem.id-1];
                    Texture& outfitTexture = m_game->m_resourceManager.GetTexture(outfitData.texturePath, 0);
                    DrawTexturePro(outfitTexture, outfitData.texturePosition, { 256, 195, 128, 128 }, { 0, 0 }, 0.0f, outfitData.isColourable ? m_game->m_gameData.OutfitTintList[m_wonItem.tint-1] : WHITE);
                    rlDrawText("Clothes!", 320 - (MeasureText("Clothes!", 30) * 0.5f), 320, 30, BLACK);
                } break;
                default: break;
            }
        } break;
        default: break;
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
