#include "boardScreen.hpp"
#include "constants.hpp"
#include "game.hpp"

#include "textureManager.hpp"

#include <iostream>

BoardScreen::BoardScreen(int x, int y, int width, int height, int rows, int cols, SDL_Texture *tileTexture, SDL_Texture *freeTileTexture) {
    std::cout << "creating boardscreen" << std::endl;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->tileTexture = tileTexture;
    this->freeTileTexture = freeTileTexture;
    
    int halfButtonSize = width / 16;

    buttons.push_back({x + halfButtonSize, y + halfButtonSize, 2 * halfButtonSize, 2 * halfButtonSize, TextureManager::LoadTexture("assets/homeButton.png"), [this](){ switchToHomeScreen(); }});
    buttons.push_back({x + 4 * halfButtonSize, y + halfButtonSize, 2 * halfButtonSize, 2 * halfButtonSize, TextureManager::LoadTexture("assets/undo.png"), [this](){ board->undo(); }});

    board = nullptr;
    changeParams(rows, cols);

    scoreTextView = new TextView(x + halfButtonSize * 7, y + halfButtonSize, width - 8 * halfButtonSize, 2 * halfButtonSize, {0, 0, 0, 255}, Game::font, "0", alignment::RIGHT);

    std::cout << "boardscreen created" << std::endl;
}

BoardScreen::~BoardScreen() {
    delete board;
}

void BoardScreen::update() {
    if(clicked > 0) {
        std::cout << "clicked" << std::endl;
        if(clicked == 1) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if(mouseX != clickX || mouseY != clickY){
                SDL_Event event;
                event.type = SDL_KEYDOWN;
                if(abs(mouseX - clickX) >= abs(mouseY - clickY)) {
                    if(mouseX - clickX > 0) event.key.keysym.sym = SDLK_RIGHT;
                    else event.key.keysym.sym = SDLK_LEFT;
                }
                else {
                    if(mouseY - clickY > 0) event.key.keysym.sym = SDLK_DOWN;
                    else event.key.keysym.sym = SDLK_UP;
                }
                SDL_PushEvent(&event);
                clicked = 0;
            }
        }
        clicked--;
    }

    board->update();
    scoreTextView->setText("score: " + std::to_string(board->getScore()));
    if(sizeChanging){
        animationProgress++;
        if(animationProgress == animationTime){
            changeParams(newRows, newCols);
        }
        else if(animationProgress == 2.5 * animationTime){
            sizeChanging = false;
        }
    }
    else if(switchingToHomeScreen) {
        animationProgress++;
        if(animationProgress == 1.5 * animationTime) {
            switchingToHomeScreen = false;
            Game::switchToHomeScreen();
        }
    }
    else if(loadingAnimation) {
        animationProgress++;
        if(animationProgress == animationTime) {
            loadingAnimation = false;
        }
    }
}

void BoardScreen::render() {
    SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(Game::renderer, &rect);
    board->render();
    if(sizeChanging) {
        if(animationProgress < animationTime){
            SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255 * animationProgress / animationTime);
            SDL_RenderFillRect(Game::renderer, &rect);
        }
        else if(animationProgress < 1.5 * animationTime){
            SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255);
            SDL_RenderFillRect(Game::renderer, &rect);
        }
        else if(animationProgress < 2.5 * animationTime){
            SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255 * (2.5 * animationTime - animationProgress) / animationTime);
            SDL_RenderFillRect(Game::renderer, &rect);
        }
    }
    for(auto button: buttons){
        button.render();
    }
    scoreTextView->render();
    if(switchingToHomeScreen){
        if(animationProgress < animationTime){
            SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255 * animationProgress / animationTime);
            SDL_RenderFillRect(Game::renderer, &rect);
        }
        else{
            SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255);
            SDL_RenderFillRect(Game::renderer, &rect);
        }
    }
    else if(loadingAnimation){
        if(animationProgress < animationTime){
            SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255 * (animationTime - animationProgress) / animationTime);
            SDL_RenderFillRect(Game::renderer, &rect);
        }
    }
}

bool BoardScreen::tryHandleMove(char direction) {
    if(sizeChanging) return false;
    if(board->isMoving()){
        board->setSpeed(3);
        while(board->isMoving()) SDL_Delay(EVENT_HANDLER_SLEEP_TIME);
        board->setSpeed(1);
    }
    return board->handleMove(direction);
}

void BoardScreen::restart() {
    board->restart();
}

void BoardScreen::changeParams(int rows, int cols) {
    std::cout << "changing params" << std::endl;
    delete board;
    std::cout << "board deleted" << std::endl;

    int halfButtonSize = width / 16;

    int baseWidth = width / (5 * cols + 1);
    int baseHeight = (height - halfButtonSize * 4) / (5 * rows + 1);
    int baseSize = std::min(baseWidth, baseHeight);

    int boardWidth = (5 * cols + 1) * baseSize;
    int boardHeight = (5 * rows + 1) * baseSize;

    board = new Board(x + (width - boardWidth)/2, y + 4 * halfButtonSize, boardWidth, boardHeight, rows, cols, 4 * baseSize, baseSize, baseSize, {(Uint8)baseR, (Uint8)baseG, (Uint8)baseB, (Uint8)255}, tileTexture, freeTileTexture);
}

void BoardScreen::animateChangeParams(int rows, int cols) {
    if(sizeChanging) return;
    sizeChanging = true;
    newRows = rows;
    newCols = cols;
    animationProgress = 0;
}

void BoardScreen::onClick(int x, int y) {
    if(sizeChanging || switchingToHomeScreen) return;
    for(auto button : buttons) {
        if(button.isInside(x, y)) {
            button.onClick();
            return;
        }
    }
    clicked = clickDelay;
    clickX = x;
    clickY = y;
}

void BoardScreen::handleEvent(SDL_Event event) {
    if(event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
            case SDLK_UP:
                tryHandleMove('u');
                break;
            case SDLK_DOWN:
                tryHandleMove('d');
                break;
            case SDLK_LEFT:
                tryHandleMove('l');
                break;
            case SDLK_RIGHT:
                tryHandleMove('r');
                break;
            case SDLK_r:
                restart();
                break;
            case SDLK_u:
                board->undo();
                break;
            case SDLK_1:
                animateChangeParams(4, 4);
                break;
            case SDLK_2:
                animateChangeParams(5, 5);
                break;
            case SDLK_3:
                animateChangeParams(6, 6);
                break;
            case SDLK_4:
                animateChangeParams(7, 7);
                break;
            case SDLK_5:
                animateChangeParams(8, 8);
                break;
            case SDLK_6:
                animateChangeParams(9, 9);
                break;
            case SDLK_7:
                animateChangeParams(10, 10);
                break;
            case SDLK_8:
                animateChangeParams(11, 11);
                break;
            case SDLK_9:
                animateChangeParams(12, 12);
                break;
            case SDLK_0:
                animateChangeParams(3, 3);
                break;
            case SDLK_ESCAPE:
                switchToHomeScreen();
                break;
            default:
                break;
        }
    }

    else if(event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        onClick(mouseX, mouseY);
    }

    else if(event.type == SDL_FINGERDOWN){
        if(event.tfinger.x * width >= x && event.tfinger.x * SCREEN_WIDTH <= x + width && event.tfinger.y * SCREEN_HEIGHT >= y && event.tfinger.y * SCREEN_HEIGHT <= y + height) {
            onClick(event.tfinger.x * SCREEN_WIDTH, event.tfinger.y * SCREEN_HEIGHT);
        }
    }
}

void BoardScreen::switchToHomeScreen() {
    sizeChanging = false;
    loadingAnimation = false;
    switchingToHomeScreen = true;
    animationProgress = 0;
}

void BoardScreen::animateLoading() {

    if(sizeChanging || switchingToHomeScreen) return;
    animationProgress = 0;
    loadingAnimation = true;
}