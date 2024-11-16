#pragma once

#include <SDL2/SDL.h>

class Help{
    private:
        int x, y, width, height;

        SDL_Texture *tileTexture;
        SDL_Texture *unknownTileTexture;
        SDL_Texture *nextTexture;

        int unlocked;
        
        int rows, cols;
        int xmargin, ymargin;
        int rowHeight, colWidth;
        int quartertilesize;

    public:
        Help(int x, int y, int width, int height, SDL_Texture *tileTexture, SDL_Texture *unknownTileTexture, SDL_Texture *nextTexture);
        ~Help();

        void update();
        void render();
};