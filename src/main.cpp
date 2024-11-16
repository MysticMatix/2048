#include "game.hpp"
#include "constants.hpp"

Game *game = nullptr;

int main(int argc, char *argv[]) {
    game = new Game();

    game->init("Colors (2048)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

    while(game->running()) {
        game->gameLoop();
    }

    game->clean();

    return 0;
}