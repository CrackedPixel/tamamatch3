#include "scene_credits.hpp"

#include "game.hpp"

void SceneCredits::OnInitialize() {

}

void SceneCredits::OnUpdate(float deltaTime) {
    if (m_game->IsChangingScenes()) {
        return;
    }
}

void SceneCredits::OnRender() {
    rlDrawText("CREDITS", 50, 50, 50, BLACK);
}

bool SceneCredits::OnHandleInput(Vector2 mousePos) {
    return false;
}
