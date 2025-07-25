#include "game.hpp"

int main() {
    Game game;

    game.OnInitialize();
    game.OnGameLoopStart();
    game.OnTerminate();

    return 0;
}
