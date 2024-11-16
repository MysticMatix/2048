#include "textureManager.hpp"

#include "game.hpp"
#include <SDL2/SDL_image.h>

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    SDL_Surface* surface = IMG_Load(fileName);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}