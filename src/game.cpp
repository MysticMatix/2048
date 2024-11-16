#include "game.hpp"

#include "constants.hpp"
#include "textureManager.hpp"

#include <iostream>

Game::Game() {
    isRunning = false;
    window = nullptr;
    renderer = nullptr;
}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if(fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        std::cout << "Display mode: " << DM.w << "x" << DM.h << std::endl;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems initialized..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window) {
            std::cout << "Window created..." << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_GetRendererOutputSize(renderer, &width, &height);
            SCREEN_WIDTH = width;
            SCREEN_HEIGHT = height;
            std::cout << "Renderer created..." << std::endl;
        }

        isRunning = true;
    } else {
        isRunning = false;
    }

    if(TTF_Init() == -1) {
        std::cout << "Error : SDL_TTF" << std::endl;
    }

    font = TTF_OpenFont("assets/Basic-Regular.ttf", 96);

    boardScreen = new BoardScreen(0, 0, width, height, 4, 4, TextureManager::LoadTexture("assets/tiles.png"), TextureManager::LoadTexture("assets/tile.png"));
    homeScreen = new HomeScreen(0, 0, width, height, boardScreen);

    gameTime = 0;

    eventHandler = std::async(std::launch::async, &Game::handleEvents, this);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    homeScreenVisible = true;

    std::cout << "Game initialized..." << std::endl;
}

void Game::handleEvents() {
    while(isRunning) {
        SDL_PollEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            default:
                if(homeScreenVisible) {
                    homeScreen->handleEvent(event);
                }
                else {
                    boardScreen->handleEvent(event);
                }
        }
        SDL_Delay(EVENT_HANDLER_SLEEP_TIME);
    }
}

void Game::update() {
    if(homeScreenVisible) {
        SDL_SetWindowTitle(window, "Colors (2048)");
        homeScreen->update();
    }
    else {
        if(boardScreen->getScore() > 0){
            SDL_SetWindowTitle(window, ("Score: " + std::to_string(boardScreen->getScore())).c_str());
        }
        else{
            SDL_SetWindowTitle(window, "Colors (2048)");
        }
        boardScreen->update();
    }

}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if(homeScreenVisible) {
        homeScreen->render();
    }
    else {
        boardScreen->render();
    }
    SDL_RenderPresent(renderer);
}

void Game::gameLoop() {
    Uint64 frameStart = SDL_GetTicks64();

    update();
    render();

    Uint64 frameTime = SDL_GetTicks64() - frameStart;

    if(frameTime < FRAME_TARGET_TIME) {
        SDL_Delay(FRAME_TARGET_TIME - frameTime);
    }
    else{
        std::cout << "Frame took too long! (" << frameTime << "ms)" << std::endl;
    }
}

void Game::clean() {
    std::cout << "Cleaning game..." << std::endl;
    delete boardScreen;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned..." << std::endl;
}

void Game::switchToHomeScreen() {
    homeScreen->animateLoading();
    homeScreenVisible = true;
}

void Game::switchToBoardScreen() {
    boardScreen->animateLoading();
    homeScreenVisible = false;
}