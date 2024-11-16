#include "button.hpp"

#include "game.hpp"

void button::render() {
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(Game::renderer, texture, NULL, &rect);
}

bool button::isInside(int x, int y) {
    return x >= this->x && x <= this->x + width && y >= this->y && y <= this->y + height;
}