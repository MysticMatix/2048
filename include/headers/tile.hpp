#pragma once

#include <SDL2/SDL.h>

class Tile{
    public:
        int x;
        int y;
        int value;
        bool merging;
        double x1, y1, x2, y2;
        int spawning;
        int increaseSize = 0;
};