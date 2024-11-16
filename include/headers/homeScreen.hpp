#pragma once

#include "boardScreen.hpp"
#include "button.hpp"
#include "textView.hpp"
#include "boardPreview.hpp"
#include "help.hpp"

#include <vector>

class HomeScreen{
    private:
        int x, y, width, height;

        const int animationTime = 30;

        bool loadingAnimation = false;
        bool switchingToBoardScreen = false;
        int animationProgress = 0;

        std::vector<button> buttons;
        TextView *lastGameTextView;
        TextView *newGameTextView;
        BoardScreen *boardScreen;
        BoardPreview *boardPreview;
        Help *help;


        int newGameRows;
        int newGameCols;

        bool helpVisible = false;

        void newGame();
        void showHelp();

        void onClick(int x, int y);

        const int clickDelay = 5;
        int clicked = 0;
        int clickX, clickY;

        Uint64 lastClickTime = 0;
        const Uint64 minClickFrequency = 30;

    public:
        HomeScreen(int x, int y, int width, int height, BoardScreen *boardScreen);
        ~HomeScreen();

        void update();
        void render();

        void handleEvent(SDL_Event event);

        void animateLoading();
};