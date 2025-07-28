#pragma once

#include "scene.hpp"
#include "types.hpp"

enum struct SHOP_STATES {
    NEW,
    BROKE,
    BUYING,
    BUY_REVIEW,
};

struct SceneShop : public Scene {
    SceneShop(Game* game) : Scene(game) {}

    void OnInitialize();
    void OnUpdate(float deltaTime);
    void OnRender();
    void OnRenderUI();

private:
    SHOP_STATES m_shopState = SHOP_STATES::NEW;
    GumballItem m_wonItem = {};
    int m_itemPrice = 10;
};
