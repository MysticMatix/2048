#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct TilePreview{
    int desiredX, desiredY, desiredWidth, desiredHeight;
    int x, y, width, height;
    bool appearing, disappearing;
    int alpha;
};

class BoardPreview{
    private:
        int x, y, width, height;
        SDL_Texture *tileTexture;
        
        int maxTiles;

        int currentRows;
        int currentCols;

        const int animationTime = 15;

        TilePreview *tiles;
    public:
        BoardPreview(int x, int y, int width, int height, SDL_Texture *tileTexture, int maxTiles);
        ~BoardPreview();

        void update();
        void render();

        void setSize(int rows, int cols);
};