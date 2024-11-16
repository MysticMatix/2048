#include "board.hpp"

#include "constants.hpp"
#include "game.hpp"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Board::Board(int x, int y, int width, int height, int rows, int cols, int tileSize, int tileSpacing, int borderWidth, SDL_Color bgColor, SDL_Texture *tileTexture, SDL_Texture *freeTileTexture) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->rows = rows;
    this->cols = cols;
    this->tileSize = tileSize;
    this->tileSpacing = tileSpacing;
    this->borderWidth = borderWidth;
    this->bgColor = bgColor;
    this->tileTexture = tileTexture;
    this->freeTileTexture = freeTileTexture;

    this->tiles = new Tile[rows * cols];
    this->restart();

    lastState = getBoardStateSave();
}

Board::~Board() {
    delete[] this->tiles;
}

void Board::restart() {
    tilesMutex.lock();
    for (int i = 0; i < this->rows * this->cols; i++) {
        this->tiles[i].x = i % this->cols;
        this->tiles[i].y = i / this->cols;
        this->tiles[i].value = 0;
        this->tiles[i].merging = false;
        this->tiles[i].x1 = this->tiles[i].x;
        this->tiles[i].y1 = this->tiles[i].y;
        this->tiles[i].x2 = this->tiles[i].x;
        this->tiles[i].y2 = this->tiles[i].y;
        this->tiles[i].spawning = 0;
    }
    this->addNewTile();
    this->addNewTile();
    this->score = 0;
    tilesMutex.unlock();
}

void Board::update() {
    //std::cout << "lock1\n";
    tilesMutex.lock();
    this->moving = false;
    for(int i = 0; i < this->rows * this->cols; ++i){
        unlockedTiles = std::max(unlockedTiles, this->tiles[i].value);
        if(this->tiles[i].spawning > 0){
            this->tiles[i].spawning -= this->speed;
            if(this->tiles[i].spawning < 0) this->tiles[i].spawning = 0;
            this->moving = true;
            continue;
        }
        else if(this->tiles[i].increaseSize != 0){
            if(this->tiles[i].increaseSize > 0){
                this->tiles[i].increaseSize /= pow(2, this->speed);
                if(this->tiles[i].increaseSize < 0) this->tiles[i].increaseSize = 0;
                this->moving = true;
            }
            else{
                this->tiles[i].increaseSize *= pow(2, this->speed);
                if(this->tiles[i].increaseSize < -sizeInc) this->tiles[i].increaseSize = sizeInc;
                this->moving = true;
            }
            continue;
        }
        if(this->tiles[i].merging){
            //std::cout << "Merging tile " << i << "(" << tiles[i].x << ", " << tiles[i].y << ")" << "[" << tiles[i].x1 << ", " << tiles[i].y1 << "]" << "[" << tiles[i].x2 << ", " << tiles[i].y2 << "]" << std::endl;
            bool together = true;
            if(this->tiles[i].x1 > this->tiles[i].x + TILE_MOVE_SPEED * speed){
                this->tiles[i].x1 -= TILE_MOVE_SPEED * speed;
                together = false;
                this->moving = true;
            }
            else if(this->tiles[i].x1 < this->tiles[i].x - TILE_MOVE_SPEED * speed){
                this->tiles[i].x1 += TILE_MOVE_SPEED * speed;
                together = false;
                this->moving = true;
            }
            else{
                this->tiles[i].x1 = this->tiles[i].x;
            }

            if(this->tiles[i].y1 > this->tiles[i].y + TILE_MOVE_SPEED * speed){
                this->tiles[i].y1 -= TILE_MOVE_SPEED * speed;
                together = false;
                this->moving = true;
            }
            else if(this->tiles[i].y1 < this->tiles[i].y - TILE_MOVE_SPEED * speed){
                this->tiles[i].y1 += TILE_MOVE_SPEED * speed;
                together = false;
                this->moving = true;
            }
            else{
                this->tiles[i].y1 = this->tiles[i].y;
            }

            if(this->tiles[i].x2 > this->tiles[i].x + TILE_MOVE_SPEED * speed){
                this->tiles[i].x2 -= TILE_MOVE_SPEED * speed;
                together = false;
                this->moving = true;
            }
            else if(this->tiles[i].x2 < this->tiles[i].x - TILE_MOVE_SPEED * speed){
                this->tiles[i].x2 += TILE_MOVE_SPEED * speed;
                together = false;
                this->moving = true;
            }
            else{
                this->tiles[i].x2 = this->tiles[i].x;
            }

            if(this->tiles[i].y2 > this->tiles[i].y + TILE_MOVE_SPEED * speed){
                this->tiles[i].y2 -= TILE_MOVE_SPEED * speed;
                together = false;
                this->moving = true;
            }
            else if(this->tiles[i].y2 < this->tiles[i].y - TILE_MOVE_SPEED * speed){
                this->tiles[i].y2 += TILE_MOVE_SPEED * speed;
                together = false;
                this->moving = true;
            }
            else{
                this->tiles[i].y2 = this->tiles[i].y;
            }

            if(together){
                this->score += pow(2, this->tiles[i].value);
                this->tiles[i].merging = false;
                this->tiles[i].x1 = this->tiles[i].x;
                this->tiles[i].y1 = this->tiles[i].y;
                this->tiles[i].x2 = this->tiles[i].x;
                this->tiles[i].y2 = this->tiles[i].y;
                //this->tiles[i].value++;
                this->tiles[i].increaseSize = -1;
            }
        }
        else{
            if(this->tiles[i].x1 > this->tiles[i].x + TILE_MOVE_SPEED * speed){
                this->tiles[i].x1 -= TILE_MOVE_SPEED * speed;
                this->moving = true;
            }
            else if(this->tiles[i].x1 < this->tiles[i].x - TILE_MOVE_SPEED * speed){
                this->tiles[i].x1 += TILE_MOVE_SPEED * speed;
                this->moving = true;
            }
            else{
                this->tiles[i].x1 = this->tiles[i].x;
            }

            if(this->tiles[i].y1 > this->tiles[i].y + TILE_MOVE_SPEED * speed){
                this->tiles[i].y1 -= TILE_MOVE_SPEED * speed;
                this->moving = true;
            }
            else if(this->tiles[i].y1 < this->tiles[i].y - TILE_MOVE_SPEED * speed){
                this->tiles[i].y1 += TILE_MOVE_SPEED * speed;
                this->moving = true;
            }
            else{
                this->tiles[i].y1 = this->tiles[i].y;
            }

            if(this->tiles[i].x2 > this->tiles[i].x + TILE_MOVE_SPEED * speed){
                this->tiles[i].x2 -= TILE_MOVE_SPEED * speed;
                this->moving = true;
            }
            else if(this->tiles[i].x2 < this->tiles[i].x - TILE_MOVE_SPEED * speed){
                this->tiles[i].x2 += TILE_MOVE_SPEED * speed;
                this->moving = true;
            }
            else{
                this->tiles[i].x2 = this->tiles[i].x;
            }

            if(this->tiles[i].y2 > this->tiles[i].y + TILE_MOVE_SPEED * speed){
                this->tiles[i].y2 -= TILE_MOVE_SPEED * speed;
                this->moving = true;
            }
            else if(this->tiles[i].y2 < this->tiles[i].y - TILE_MOVE_SPEED * speed){
                this->tiles[i].y2 += TILE_MOVE_SPEED * speed;
                this->moving = true;
            }
            else{
                this->tiles[i].y2 = this->tiles[i].y;
            }
        }
    }
    tilesMutex.unlock();
}

void Board::render() {
    SDL_Rect boardRect = {this->x, this->y, this->width, this->height};
    SDL_SetRenderDrawColor(Game::renderer, this->bgColor.r, this->bgColor.g, this->bgColor.b, this->bgColor.a);
    SDL_RenderFillRect(Game::renderer, &boardRect);

    //std::cout << "Rendering board" << std::endl;

    for (int i = 0; i < this->rows * this->cols; i++) {
        SDL_Rect srcRect = {0,0,64,64};
        SDL_Rect destRect = {this->x + this->tiles[i].x * (this->tileSize + this->tileSpacing) + this->borderWidth, this->y + this->tiles[i].y * (this->tileSize + this->tileSpacing) + this->borderWidth, this->tileSize, this->tileSize};
        SDL_RenderCopy(Game::renderer, this->freeTileTexture, &srcRect, &destRect);
        if(this->tiles[i].spawning > 0){
            if(this->tiles[i].spawning < SPAWN_ANIMATION_TIME / 2){
                int change = this->tiles[i].spawning * 2 * tileSize / SPAWN_ANIMATION_TIME;
                destRect.x += change / 2;
                destRect.y += change / 2;
                destRect.w -= change;
                destRect.h -= change;
                SDL_RenderCopy(Game::renderer, this->tileTexture, &srcRect, &destRect);
            }
        }
    }

    for (int i = 0; i < this->rows * this->cols; i++) {
        SDL_assert(this->tiles[i].value >= 0);

        //std::cout << "Rendering tile " << i << "(" << tiles[i].x << ", " << tiles[i].y << ")" << std::endl;


        if (this->tiles[i].value == 0) continue;

        SDL_Rect srcRect = {this->tiles[i].value * 64 - 64, 0, 64, 64};
        SDL_Rect destRect = {(int)(this->x + this->tiles[i].x1 * (this->tileSize + this->tileSpacing) + this->borderWidth), (int)(this->y + this->tiles[i].y1 * (this->tileSize + this->tileSpacing) + this->borderWidth), this->tileSize, this->tileSize};
        
        if(this->tiles[i].spawning > 0){
            continue;
        }

        if(this->tiles[i].increaseSize != 0){
            int change = abs(this->tiles[i].increaseSize);
            destRect.x -= change / 2;
            destRect.y -= change / 2;
            destRect.w += change;
            destRect.h += change;
            SDL_RenderCopy(Game::renderer, this->tileTexture, &srcRect, &destRect);
            continue;
        }
        
        //std::cout << "not spawning" << std::endl;

        
        if(this->tiles[i].merging){
            srcRect.x -= 64;
            SDL_RenderCopy(Game::renderer, this->tileTexture, &srcRect, &destRect);
            destRect = {(int)(this->x + this->tiles[i].x2 * (this->tileSize + this->tileSpacing) + this->borderWidth), (int)(this->y + this->tiles[i].y2 * (this->tileSize + this->tileSpacing) + this->borderWidth), this->tileSize, this->tileSize};
            SDL_RenderCopy(Game::renderer, this->tileTexture, &srcRect, &destRect);
        }
        else{
            SDL_RenderCopy(Game::renderer, this->tileTexture, &srcRect, &destRect);
        }
    }
}

bool Board::isMoving() {
    return moving;
}

void Board::skipAnimation() {
    //std::cout << "lock2\n";
    tilesMutex.lock();
    for(int i = 0; i < this->rows * this->cols; ++i){
        this->tiles[i].spawning = 0;
        this->tiles[i].x1 = this->tiles[i].x;
        this->tiles[i].y1 = this->tiles[i].y;
        this->tiles[i].x2 = this->tiles[i].x;
        this->tiles[i].y2 = this->tiles[i].y;
        if(this->tiles[i].merging){
            this->tiles[i].merging = false;
            this->tiles[i].value++;
        }
    }
    moving = false;
    tilesMutex.unlock();
}

void Board::addNewTile() {
    //std::cout << "lock3\n";
    //tilesMutex.lock();
    //count empty tiles
    int emptyTiles = 0;
    for (int i = 0; i < this->rows * this->cols; i++) {
        if (this->tiles[i].value == 0) emptyTiles++;
    }

    if(emptyTiles == 0){
        //std::cout << "unlock3\n";
        tilesMutex.unlock();
        return;
    }

    //choose random empty tile
    int randomTile = rand() % emptyTiles;
    for (int i = 0; i < this->rows * this->cols; i++) {
        if (this->tiles[i].value == 0) {
            if (randomTile == 0) {
                this->tiles[i].value = 1;
                this->tiles[i].merging = false;
                this->tiles[i].x1 = this->tiles[i].x;
                this->tiles[i].y1 = this->tiles[i].y;
                this->tiles[i].x2 = this->tiles[i].x;
                this->tiles[i].y2 = this->tiles[i].y;
                this->tiles[i].spawning = SPAWN_ANIMATION_TIME;
                //std::cout << "unlock3\n";
                tilesMutex.unlock();
                return;
            }
            randomTile--;
        }
    }
    //std::cout << "unlock3\n";
    //tilesMutex.unlock();
}

bool Board::isFull() {
    //std::cout << "lock4\n";
    tilesMutex.lock();
    for (int i = 0; i < this->rows * this->cols; i++) {
        if (this->tiles[i].value == 0){
            tilesMutex.unlock();
            return false;
        }
    }
    tilesMutex.unlock();
    return true;
}

bool Board::handleMove(char direction) {
    //std::cout << "lock5\n";
    boardStateSave state = getBoardStateSave();
    tilesMutex.lock();
    bool moved = false;
    switch (direction) {
        case 'u':
            moved = this->moveUp();
            break;
        case 'd':
            moved = this->moveDown();
            break;
        case 'l':
            moved = this->moveLeft();
            break;
        case 'r':
            moved = this->moveRight();
            break;
    }
    //std::cout << "unlock5\n";
    tilesMutex.unlock();
    if(moved){
        lastState = state;
    }
    return moved;
}

bool Board::moveUp() {
    if(this->moving) return false;
    bool moved = false;
    for (int i = 0; i < this->cols; i++) {
        for (int j = 1; j < this->rows; j++) {
            if (this->tiles[j * this->cols + i].value == 0) continue;
            int k = j;
            while (k > 0 && this->tiles[(k - 1) * this->cols + i].value == 0) {
                this->tiles[(k - 1) * this->cols + i].value = this->tiles[k * this->cols + i].value;
                this->tiles[(k - 1) * this->cols + i].x1 = this->tiles[k * this->cols + i].x1;
                this->tiles[(k - 1) * this->cols + i].y1 = this->tiles[k * this->cols + i].y1;
                this->tiles[k * this->cols + i].value = 0;
                k--;
                moved = true;
            }
            if (k > 0 && this->tiles[(k - 1) * this->cols + i].value == this->tiles[k * this->cols + i].value && !this->tiles[(k - 1) * this->cols + i].merging && !this->tiles[k * this->cols + i].merging) {
                this->tiles[(k - 1) * this->cols + i].value++;
                this->tiles[(k - 1) * this->cols + i].merging = true;
                this->tiles[(k - 1) * this->cols + i].x2 = this->tiles[k * this->cols + i].x1;
                this->tiles[(k - 1) * this->cols + i].y2 = this->tiles[k * this->cols + i].y1;
                this->tiles[k * this->cols + i].value = 0;
                moved = true;
            }
        }
    }
    if(moved) this->addNewTile();
    return moved;
}

bool Board::moveDown() {
    if(this->moving) return false;
    bool moved = false;
    for (int i = 0; i < this->cols; i++) {
        for (int j = this->rows - 2; j >= 0; j--) {
            if (this->tiles[j * this->cols + i].value == 0) continue;
            int k = j;
            while (k < this->rows - 1 && this->tiles[(k + 1) * this->cols + i].value == 0) {
                this->tiles[(k + 1) * this->cols + i].value = this->tiles[k * this->cols + i].value;
                this->tiles[(k + 1) * this->cols + i].x1 = this->tiles[k * this->cols + i].x1;
                this->tiles[(k + 1) * this->cols + i].y1 = this->tiles[k * this->cols + i].y1;
                this->tiles[k * this->cols + i].value = 0;
                k++;
                moved = true;
            }
            if (k < this->rows - 1 && this->tiles[(k + 1) * this->cols + i].value == this->tiles[k * this->cols + i].value && !this->tiles[(k + 1) * this->cols + i].merging && !this->tiles[k * this->cols + i].merging) {
                this->tiles[(k + 1) * this->cols + i].value++;
                this->tiles[(k + 1) * this->cols + i].merging = true;
                this->tiles[(k + 1) * this->cols + i].x2 = this->tiles[k * this->cols + i].x1;
                this->tiles[(k + 1) * this->cols + i].y2 = this->tiles[k * this->cols + i].y1;
                this->tiles[k * this->cols + i].value = 0;
                moved = true;
            }
        }
    }
    if(moved) this->addNewTile();
    return moved;
}

bool Board::moveLeft() {
    if(this->moving) return false;
    bool moved = false;
    for (int i = 0; i < this->rows; i++) {
        for (int j = 1; j < this->cols; j++) {
            if (this->tiles[i * this->cols + j].value == 0) continue;
            int k = j;
            while (k > 0 && this->tiles[i * this->cols + k - 1].value == 0) {
                this->tiles[i * this->cols + k - 1].value = this->tiles[i * this->cols + k].value;
                this->tiles[i * this->cols + k - 1].x1 = this->tiles[i * this->cols + k].x1;
                this->tiles[i * this->cols + k - 1].y1 = this->tiles[i * this->cols + k].y1;
                this->tiles[i * this->cols + k].value = 0;
                k--;
                moved = true;
            }
            if (k > 0 && this->tiles[i * this->cols + k - 1].value == this->tiles[i * this->cols + k].value && !this->tiles[i * this->cols + k - 1].merging && !this->tiles[i * this->cols + k].merging) {
                this->tiles[i * this->cols + k - 1].value++;
                this->tiles[i * this->cols + k - 1].merging = true;
                this->tiles[i * this->cols + k - 1].x2 = this->tiles[i * this->cols + k].x1;
                this->tiles[i * this->cols + k - 1].y2 = this->tiles[i * this->cols + k].y1;
                this->tiles[i * this->cols + k].value = 0;
                moved = true;
            }
        }
    }
    if(moved) this->addNewTile();
    return moved;
}

bool Board::moveRight() {
    if(this->moving) return false;
    bool moved = false;
    for (int i = 0; i < this->rows; i++) {
        for (int j = this->cols - 2; j >= 0; j--) {
            if (this->tiles[i * this->cols + j].value == 0) continue;
            int k = j;
            while (k < this->cols - 1 && this->tiles[i * this->cols + k + 1].value == 0) {
                this->tiles[i * this->cols + k + 1].value = this->tiles[i * this->cols + k].value;
                this->tiles[i * this->cols + k + 1].x1 = this->tiles[i * this->cols + k].x1;
                this->tiles[i * this->cols + k + 1].y1 = this->tiles[i * this->cols + k].y1;
                this->tiles[i * this->cols + k].value = 0;
                k++;
                moved = true;
            }
            if (k < this->cols - 1 && this->tiles[i * this->cols + k + 1].value == this->tiles[i * this->cols + k].value && !this->tiles[i * this->cols + k + 1].merging && !this->tiles[i * this->cols + k].merging) {
                this->tiles[i * this->cols + k + 1].value++;
                this->tiles[i * this->cols + k + 1].merging = true;
                this->tiles[i * this->cols + k + 1].x2 = this->tiles[i * this->cols + k].x1;
                this->tiles[i * this->cols + k + 1].y2 = this->tiles[i * this->cols + k].y1;
                this->tiles[i * this->cols + k].value = 0;
                moved = true;
            }
        }
    }
    if(moved) this->addNewTile();
    return moved;
}

void Board::undo() {
    setBoardStateSave(lastState);
}

boardStateSave Board::getBoardStateSave() {
    boardStateSave state;
    state.rows = this->rows;
    state.cols = this->cols;
    tilesMutex.lock();
    for(int i = 0; i < this->rows * this->cols; ++i){
        state.values.push_back(this->tiles[i].value);
    }
    state.score = this->score;
    tilesMutex.unlock();
    return state;
}

void Board::setBoardStateSave(boardStateSave state) {
    tilesMutex.lock();
    if(this->rows != state.rows || this->cols != state.cols){
        this->rows = state.rows;
        this->cols = state.cols;
        delete[] this->tiles;
        this->tiles = new Tile[this->rows * this->cols];
    }
    for(int i = 0; i < this->rows * this->cols; ++i){
        this->tiles[i].value = state.values[i];
        this->tiles[i].merging = false;
        this->tiles[i].spawning = 0;
        this->tiles[i].x1 = this->tiles[i].x;
        this->tiles[i].y1 = this->tiles[i].y;
        this->tiles[i].x2 = this->tiles[i].x;
        this->tiles[i].y2 = this->tiles[i].y;
        this->tiles[i].increaseSize = 0;
    }
    this->score = state.score;
    tilesMutex.unlock();
}