#pragma once

#include <vector>
#include <string>

#include "scene.hpp"

struct SceneMenu : public Scene {
    SceneMenu(Game* game) : Scene(game) {}

    void OnInitialize();
    void OnUpdate(float deltaTime);
    void OnRender();

private:
    static constexpr const int FONT_SIZE = 20;
    static constexpr const int START_Y = 250;
    static constexpr const float SPACE_Y = FONT_SIZE * 2.5f;
    std::vector<std::string> m_menuList = { "New Game", "Load Game", "Credits", "Quit" };
    int m_selectionId = 0;
};
