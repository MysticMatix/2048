#include "homeScreen.hpp"

#include "constants.hpp"
#include "game.hpp"
#include "textureManager.hpp"

#include <iostream>

HomeScreen::HomeScreen(int x, int y, int width, int height, BoardScreen *boardScreen) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->boardScreen = boardScreen;
    this->newGameCols = 4;
    this->newGameRows = 4;
    
    help = new Help(x, y, width, height, TextureManager::LoadTexture("assets/tiles.png"), TextureManager::LoadTexture("assets/unknown.png"), TextureManager::LoadTexture("assets/next.png"));

    int halfButtonSize = width / 16;

    buttons.push_back({x + width - 3 * halfButtonSize, y + halfButtonSize, 2 * halfButtonSize, 2 * halfButtonSize, TextureManager::LoadTexture("assets/resume.png"), [this](){ 
        if(loadingAnimation || switchingToBoardScreen) return;
        animationProgress = 0;
        switchingToBoardScreen = true;
    }});

    buttons.push_back({x + halfButtonSize, y + halfButtonSize, 2 * halfButtonSize, 2 * halfButtonSize, TextureManager::LoadTexture("assets/help.png"), [this](){
        showHelp();
    }});

    int newGameY = y + height - 4 * halfButtonSize;

    buttons.push_back({x + 5 * halfButtonSize, newGameY, 6 * halfButtonSize, 2 * halfButtonSize, TextureManager::LoadTexture("assets/newGame.png"), [this](){
        newGame();
    }});

    buttons.push_back({x + 2 * halfButtonSize, newGameY, 2 * halfButtonSize, 2 * halfButtonSize, TextureManager::LoadTexture("assets/left.png"), [this](){
        if(std::min(newGameRows, newGameCols) > 2) {
            newGameRows--;
            newGameCols--;
        }
    }});

    buttons.push_back({x + 12 * halfButtonSize, newGameY, 2 * halfButtonSize, 2 * halfButtonSize, TextureManager::LoadTexture("assets/right.png"), [this](){
        if(std::max(newGameRows, newGameCols) < 8) {
            newGameRows++;
            newGameCols++;
        }
    }});

    int boardPreviewSize = std::min(width - 2 * halfButtonSize, height - 11 * halfButtonSize);
    int boardPrieviewMargin = (width - boardPreviewSize) / 2;

    boardPreview = new BoardPreview(x + boardPrieviewMargin, newGameY - 4 * halfButtonSize - boardPreviewSize, boardPreviewSize, boardPreviewSize, TextureManager::LoadTexture("assets/tile.png"), 64);
    boardPreview->setSize(4, 4);

    lastGameTextView = new TextView(x + 4 * halfButtonSize, y + halfButtonSize, width - 8 * halfButtonSize, 2 * halfButtonSize, {0, 0, 0, 255}, Game::font, "Last game: ", alignment::RIGHT);
    newGameTextView = new TextView(x + 2 * halfButtonSize, newGameY - 3 * halfButtonSize, 12 * halfButtonSize, 2 * halfButtonSize, {0, 0, 0, 255}, Game::font, "4x4", alignment::CENTER);
}

HomeScreen::~HomeScreen() {
    
}

void HomeScreen::showHelp() {
    helpVisible = true;
    std::cout << unlockedTiles << std::endl;
}

void HomeScreen::newGame() {
    if(loadingAnimation || switchingToBoardScreen) return;
    boardScreen->changeParams(newGameRows, newGameCols);
    animationProgress = 0;
    switchingToBoardScreen = true;
}

void HomeScreen::update() {
    if(helpVisible) help->update();
    if(clicked > 0) {
        if(clicked == 1) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if(mouseX != clickX || mouseY != clickY){
                SDL_Event event;
                event.type = SDL_KEYDOWN;
                if(mouseX - clickX > 0) event.key.keysym.sym = SDLK_LEFT;
                else event.key.keysym.sym = SDLK_RIGHT;
                SDL_PushEvent(&event);
                clicked = 0;
            }
        }
        clicked--;
    }

    boardPreview->setSize(newGameRows, newGameCols);
    boardPreview->update();
    newGameTextView->setText(std::to_string(newGameRows) + "x" + std::to_string(newGameCols));
    lastGameTextView->setText(std::to_string(boardScreen->getRows()) + "x" + std::to_string(boardScreen->getCols()) + ": " + std::to_string(boardScreen->getScore()));
    if(loadingAnimation) {
        animationProgress++;
        if(animationProgress == animationTime) {
            loadingAnimation = false;
        }
    }
    else if(switchingToBoardScreen) {
        animationProgress++;
        if(animationProgress == 1.5 * animationTime) {
            switchingToBoardScreen = false;
            Game::switchToBoardScreen();
        }
    }
}

void HomeScreen::render() {
    SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(Game::renderer, &rect);

    for(auto button : buttons) {
        button.render();
    }
    newGameTextView->render();
    lastGameTextView->render();
    boardPreview->render();
    
    if (loadingAnimation){
        SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255 * (animationTime - animationProgress) / animationTime);
        SDL_RenderFillRect(Game::renderer, &rect);
    }
    else if(switchingToBoardScreen) {
        if(animationProgress < animationTime){
            SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255 * animationProgress / animationTime);
            SDL_RenderFillRect(Game::renderer, &rect);
        }
        else{
            SDL_SetRenderDrawColor(Game::renderer, baseR, baseG, baseB, 255);
            SDL_RenderFillRect(Game::renderer, &rect);
        }
    }

    if(helpVisible){
        help->render();
    }
}

void HomeScreen::onClick(int x, int y) {
    if(SDL_GetTicks64() - lastClickTime < minClickFrequency) return;
    lastClickTime = SDL_GetTicks64();

    if(helpVisible) {
        helpVisible = false;
        return;
    }

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

void HomeScreen::handleEvent(SDL_Event event) {
    if(event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        onClick(mouseX, mouseY);
    }

    else if(event.type == SDL_FINGERDOWN){
        if(event.tfinger.x * width >= x && event.tfinger.x * SCREEN_WIDTH <= x + width && event.tfinger.y * SCREEN_HEIGHT >= y && event.tfinger.y * SCREEN_HEIGHT <= y + height) {
            onClick(event.tfinger.x * SCREEN_WIDTH, event.tfinger.y * SCREEN_HEIGHT);
        }
    }

    else if(event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
            case SDLK_LEFT:
                if(std::min(newGameRows, newGameCols) > 2) {
                    newGameRows--;
                    newGameCols--;
                }
                break;
            case SDLK_RIGHT:
                if(std::max(newGameRows, newGameCols) < 8) {
                    newGameRows++;
                    newGameCols++;
                }
                break;
            case SDLK_RETURN:
                newGame();
                break;
            case SDLK_SPACE:
                if(loadingAnimation || switchingToBoardScreen) return;
                animationProgress = 0;
                switchingToBoardScreen = true;
                break;
            default:
                break;
        }
    }
}

void HomeScreen::animateLoading() {
    loadingAnimation = true;
    animationProgress = 0;
}