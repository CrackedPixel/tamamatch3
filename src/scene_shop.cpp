#include "scene_shop.hpp"

#include "game.hpp"

void SceneShop::OnInitialize() {
    m_shopState = SHOP_STATES::NEW;
}

void SceneShop::OnUpdate(float deltaTime) {
    if (m_game->m_inputController.IsButtonBack) {
        m_game->ChangeScene("tama", true, 0.5f);
        return;
    }

    if (m_game->m_inputController.IsButtonSelect) {
        switch (m_shopState) {
            case SHOP_STATES::NEW: {

            } break;
            case SHOP_STATES::BUYING: {

            } break;
            case SHOP_STATES::BUY_REVIEW: {

            } break;
            default: break;
        }
    }
}

void SceneShop::OnRender() {
    Texture& wallpaperTexture = m_game->m_resourceManager.GetTexture(m_game->m_gameData.GetCurrentWallpaper());

    DrawTexture(wallpaperTexture, 0, 0, WHITE);
}

void SceneShop::OnRenderUI() {
    switch (m_shopState) {
        case SHOP_STATES::NEW: {

        } break;
        case SHOP_STATES::BUYING: {

        } break;
        case SHOP_STATES::BUY_REVIEW: {

        } break;
        default: break;
    }
}
