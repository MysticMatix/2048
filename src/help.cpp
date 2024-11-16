#include "help.hpp"

#include "game.hpp"
#include "constants.hpp"

Help::Help(int x, int y, int width, int height, SDL_Texture *tileTexture, SDL_Texture *unknownTileTexture, SDL_Texture *nextTexture){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->tileTexture = tileTexture;
    this->unknownTileTexture = unknownTileTexture;
    this->nextTexture = nextTexture;
}

Help::~Help(){

}

void Help::update(){
    unlocked = unlockedTiles;
    cols = 2;

    quartertilesize = width / 46;
    xmargin = quartertilesize * 2;
    ymargin = quartertilesize * 2;
    colWidth = 23 * quartertilesize;
    rowHeight = 6 * quartertilesize;

    rows = (height - 3 * quartertilesize) / rowHeight;
}

void Help::render(){
    SDL_Rect fullRect = {x, y, width, height};
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(Game::renderer, &fullRect);

    for(int i = 0; i< unlocked; i++){
        int col = i / rows;
        int row = i % rows;

        SDL_Rect srcRect = {64 * i, 0, 64, 64};

        SDL_Rect rect = {x + xmargin + col * colWidth, y + ymargin + row * rowHeight, quartertilesize * 4, quartertilesize * 4};

        SDL_RenderCopy(Game::renderer, tileTexture, &srcRect, &rect);

        rect.x += quartertilesize * 5;

        SDL_RenderCopy(Game::renderer, tileTexture, &srcRect, &rect);

        rect.x += quartertilesize * 5;

        srcRect.x = 0;
        SDL_RenderCopy(Game::renderer, nextTexture, &srcRect, &rect);

        rect.x += quartertilesize * 5;
        
        if(i + 1 < unlocked){
            srcRect.x = 64 * (i + 1);
            SDL_RenderCopy(Game::renderer, tileTexture, &srcRect, &rect);
        }
        else{
            srcRect.x = 0;
            SDL_RenderCopy(Game::renderer, unknownTileTexture, &srcRect, &rect);
        }
    }
}