#pragma once

#include <SDL2/SDL.h>
#include <functional>

struct button{
    int x, y, width, height;
    SDL_Texture *texture;
    std::function<void()> onClick;
    void render();
    bool isInside(int x, int y);
};