#include "boardPreview.hpp"

#include "constants.hpp"
#include "game.hpp"

#include <iostream>

BoardPreview::BoardPreview(int x, int y, int width, int height, SDL_Texture *tileTexture, int maxTiles){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->tileTexture = tileTexture;
    this->maxTiles = maxTiles;

    this->currentCols = 0;
    this->currentRows = 0;

    tiles = new TilePreview[maxTiles];
    for(int i = 0; i < maxTiles; i++){
        tiles[i].appearing = false;
        tiles[i].disappearing = false;
        tiles[i].alpha = 0;
        tiles[i].desiredX = tiles[i].x = 0;
        tiles[i].desiredY = tiles[i].y = 0;
        tiles[i].desiredWidth = tiles[i].width = 1;
        tiles[i].desiredHeight = tiles[i].height = 1;
    }
}

BoardPreview::~BoardPreview(){
    delete[] tiles;
}

void BoardPreview::update(){
    for(int i = 0; i < maxTiles; i++){
        if(tiles[i].appearing){
            tiles[i].alpha += 255 / animationTime;
            if(tiles[i].alpha >= 255){
                tiles[i].alpha = 255;
                tiles[i].appearing = false;
            }
        }
        else if(tiles[i].disappearing){
            tiles[i].alpha -= 255 / animationTime;
            if(tiles[i].alpha <= 0){
                tiles[i].alpha = 0;
                tiles[i].disappearing = false;
            }
        }

        if(tiles[i].desiredX != tiles[i].x){
            int dif = (tiles[i].desiredX - tiles[i].x) / 10;
            if(dif == 0){
                if(tiles[i].x < tiles[i].desiredX) tiles[i].x++;
                else tiles[i].x--;
            }
            else tiles[i].x += dif;
        }
        if(tiles[i].desiredY != tiles[i].y){
            int dif = (tiles[i].desiredY - tiles[i].y) / 10;
            if(dif == 0){
                if(tiles[i].y < tiles[i].desiredY) tiles[i].y++;
                else tiles[i].y--;
            }
            else tiles[i].y += dif;
        }
        if(tiles[i].desiredWidth != tiles[i].width){
            int dif = (tiles[i].desiredWidth - tiles[i].width) / 10;
            if(dif == 0){
                if(tiles[i].width < tiles[i].desiredWidth) tiles[i].width++;
                else tiles[i].width--;
            }
            else tiles[i].width += dif;
        }
        if(tiles[i].desiredHeight != tiles[i].height){
            int dif = (tiles[i].desiredHeight - tiles[i].height) / 10;
            if(dif == 0){
                if(tiles[i].height < tiles[i].desiredHeight) tiles[i].height++;
                else tiles[i].height--;
            }
            else tiles[i].height += dif;
        }
    }
}

void BoardPreview::render(){
    //std::cout << "rendering board preview " << currentRows << " " << currentCols;
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255);
    SDL_RenderFillRect(Game::renderer, &rect);

    int tileWidth = width / currentCols;
    int tileHeight = height / currentRows;

    for(int i = 0; i < maxTiles; i++){
        SDL_Rect rect;
        rect.x = x + tiles[i].x;
        rect.y = y + tiles[i].y;
        rect.w = tiles[i].width;
        rect.h = tiles[i].height;
        SDL_SetTextureAlphaMod(tileTexture, tiles[i].alpha);
        SDL_RenderCopy(Game::renderer, tileTexture, NULL, &rect);
    }
}

void BoardPreview::setSize(int rows, int cols){
    if(rows * cols > maxTiles){
        std::cout << "BoardPreview::setSize: too many tiles" << std::endl;
        return;
    }
    if(rows == currentRows && cols == currentCols) return;
    int desiredSize = std::min(width / cols, height / rows);
    for(int i = rows * cols; i < currentRows * currentCols; ++i){
        tiles[i].appearing = false;
        tiles[i].disappearing = true;
    }
    for(int i = currentRows * currentCols; i < rows * cols; ++i){
        tiles[i].appearing = true;
        tiles[i].disappearing = false;
    }
    std::random_shuffle(tiles, tiles + rows * cols);
    for(int i = 0; i < rows * cols; i++){
        tiles[i].desiredX = int(i % cols) * desiredSize;
        tiles[i].desiredY = int(i / cols) * desiredSize;
        tiles[i].desiredWidth = desiredSize;
        tiles[i].desiredHeight = desiredSize;
    }
    currentRows = rows;
    currentCols = cols;
}
