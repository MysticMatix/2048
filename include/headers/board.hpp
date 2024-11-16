#pragma once

#include <SDL2/SDL.h>
#include "tile.hpp"
#include <vector>
#include <mutex>

struct boardStateSave{
    int rows, cols;
    std::vector<int> values;
    int score;
};

class Board {
    private:
        int x;
        int y;
        int width;
        int height;
        int rows;
        int cols;
        int tileSize;
        int tileSpacing;
        int borderWidth;
        SDL_Color bgColor;

        Tile *tiles;

        bool moving;

        void addNewTile();
        bool isFull();

        int speed = 1;
        int sizeInc = 32;

        int score;

        std::mutex tilesMutex;

        bool moveUp();
        bool moveDown();
        bool moveLeft();
        bool moveRight();

        boardStateSave lastState;
    public:
        void setSpeed(int speed) { this->speed = speed; }
        Board(int x, int y, int width, int height, int rows, int cols, int tileSize, int tileSpacing, int borderWidth, SDL_Color bgColor, SDL_Texture *tileTexture, SDL_Texture *freeTileTexture);
        ~Board();

        void restart();
        void update();
        void render();

        bool handleMove(char direction);

        bool isMoving();
        void skipAnimation();

        int getScore() { return score; }

        SDL_Texture* freeTileTexture;
        SDL_Texture *tileTexture;

        void undo();

        boardStateSave getBoardStateSave();
        void setBoardStateSave(boardStateSave state);

        int getRows() { return rows; }
        int getCols() { return cols; }
};