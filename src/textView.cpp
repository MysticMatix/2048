#include "textView.hpp"

#include "game.hpp"

TextView::TextView(int x, int y, int width, int height, SDL_Color color, TTF_Font *font, std::string text, alignment align){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
    this->font = font;
    this->text = text;
    this->align = align;
    
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    textWidth = surface->w;
    textHeight = surface->h;
    texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
}

TextView::~TextView(){
    SDL_DestroyTexture(texture);
}

void TextView::update(){

}

void TextView::render(){
    if(visible){
        SDL_RenderCopy(Game::renderer, texture, NULL, &destRect);
    }
}

void TextView::setText(std::string text){
    this->text = text;
    SDL_DestroyTexture(texture);
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    textWidth = surface->w;
    textHeight = surface->h;
    texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    double scale = std::min((double)width / textWidth, (double)height / textHeight);
    switch (align){
        case alignment::LEFT:
            destRect = {x, y, (int)(textWidth * scale), (int)(textHeight * scale)};
            break;
        case alignment::CENTER:
            destRect = {(int)(x + width / 2 - textWidth * scale / 2), (int)(y + height / 2 - textHeight * scale / 2),(int)(textWidth * scale), (int)(textHeight * scale)};
            break;
        case alignment::RIGHT:
            destRect = {(int)(x + width - textWidth * scale), (int)(y + height - textHeight * scale), (int)(textWidth * scale), (int)(textHeight * scale)};
            break;
    }
}

void TextView::setVisible(bool visible){
    this->visible = visible;
}

void TextView::setAlign(alignment align){
    this->align = align;
    double scale = std::min((double)width / textWidth, (double)height / textHeight);
    switch (align){
        case alignment::LEFT:
            destRect = {x, y, (int)(textWidth * scale), (int)(textHeight * scale)};
            break;
        case alignment::CENTER:
            destRect = {(int)(x + width / 2 - textWidth * scale / 2), (int)(y + height / 2 - textHeight * scale / 2), (int)(textWidth * scale), (int)(textHeight * scale)};
            break;
        case alignment::RIGHT:
            destRect = {(int)(x + width - textWidth * scale), (int)(y + height - textHeight * scale), (int)(textWidth * scale), (int)(textHeight * scale)};
            break;
    }
}

void TextView::setColor(SDL_Color color){
    this->color = color;
    SDL_DestroyTexture(texture);
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    textWidth = surface->w;
    textHeight = surface->h;
    texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    double scale = std::min((double)width / textWidth, (double)height / textHeight);
    switch (align){
        case alignment::LEFT:
            destRect = {x, y, (int)(textWidth * scale), (int)(textHeight * scale)};
            break;
        case alignment::CENTER:
            destRect = {(int)(x + width / 2 - textWidth * scale / 2), (int)(y + height / 2 - textHeight * scale / 2),(int)(textWidth * scale), (int)(textHeight * scale)};
            break;
        case alignment::RIGHT:
            destRect = {(int)(x + width - textWidth * scale), (int)(y + height - textHeight * scale), (int)(textWidth * scale), (int)(textHeight * scale)};
            break;
    }
}

void TextView::setFont(TTF_Font *font){
    this->font = font;
    SDL_DestroyTexture(texture);
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    textWidth = surface->w;
    textHeight = surface->h;
    texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    double scale = std::min((double)width / textWidth, (double)height / textHeight);
    switch (align){
        case alignment::LEFT:
            destRect = {x, y, (int)(textWidth * scale), (int)(textHeight * scale)};
            break;
        case alignment::CENTER:
            destRect = {(int)(x + width / 2 - textWidth * scale / 2), (int)(y + height / 2 - textHeight * scale / 2),(int)(textWidth * scale), (int)(textHeight * scale)};
            break;
        case alignment::RIGHT:
            destRect = {(int)(x + width - textWidth * scale), (int)(y + height - textHeight * scale), (int)(textWidth * scale), (int)(textHeight * scale)};
            break;
    }
}

