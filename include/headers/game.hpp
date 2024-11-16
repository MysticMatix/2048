#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <future>

#include "boardScreen.hpp"
#include "homeScreen.hpp"

class Game {
    public:
        Game();
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
        void gameLoop();

        bool running() { return isRunning; }

        void clean();

        inline static SDL_Renderer *renderer;
        inline static SDL_Event event;

        inline static Uint64 gameTime;
        inline static std::future<void> eventHandler;

        inline static TTF_Font *font;

        static void switchToHomeScreen();
        static void switchToBoardScreen();
    private:
        inline static BoardScreen *boardScreen;
        inline static HomeScreen *homeScreen;

        bool isRunning;
        SDL_Window *window;

        void update();
        void render();

        inline static bool homeScreenVisible = false;

        void handleEvents();
};