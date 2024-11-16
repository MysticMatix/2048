#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "board.hpp"
#include "button.hpp"
#include "textView.hpp"

class BoardScreen {
    private:
        int x;
        int y;
        int width;
        int height;

        Board *board;

        SDL_Texture *tileTexture;
        SDL_Texture *freeTileTexture;

        const int animationTime = 30;

        // SDL_Texture *backgroundTexture;

        bool sizeChanging = false;
        bool switchingToHomeScreen = false;
        bool loadingAnimation = false;
        int animationProgress = 0;
        int newRows;
        int newCols;

        std::vector<button> buttons;

        TextView *scoreTextView;

        void onClick(int x, int y);

        const int clickDelay = 5;
        int clicked = 0;
        int clickX, clickY;

    public:
        BoardScreen(int x, int y, int width, int height, int rows, int cols, SDL_Texture *tileTexture, SDL_Texture *freeTileTexture);
        ~BoardScreen();

        void update();
        void render();

        bool tryHandleMove(char direction);
        
        void restart();
        void changeParams(int rows, int cols);
        void animateChangeParams(int rows, int cols);

        int getScore() { return board->getScore(); }

        void handleEvent(SDL_Event event);
        
        void switchToHomeScreen();
        void animateLoading();

        int getRows() { return board->getRows(); }
        int getCols() { return board->getCols(); }
};