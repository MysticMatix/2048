#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

enum alignment{
    LEFT,
    CENTER,
    RIGHT
};

class TextView{
    private:
        int x, y, width, height;
        SDL_Texture *texture;
        SDL_Color color;
        TTF_Font *font;
        std::string text;
        alignment align;
        bool visible = true;

        int textWidth;
        int textHeight;

        SDL_Rect destRect;

    public:
        TextView(int x, int y, int width, int height, SDL_Color color, TTF_Font *font, std::string text, alignment align);
        ~TextView();

        void update();
        void render();

        void setText(std::string text);
        void setVisible(bool visible);
        void setAlign(alignment align);
        void setColor(SDL_Color color);
        void setFont(TTF_Font *font);
};