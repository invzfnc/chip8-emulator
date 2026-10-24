#pragma once

#include "chip8.h"
#include <SDL3/SDL.h>

class Platform {

    public:
        Platform(const char *title, int window_width, int window_height);
        ~Platform();
        void update(unsigned char screen[][Chip8::SCREEN_WIDTH]);

    private:
        int pitch = 4 * Chip8::SCREEN_WIDTH;  // 4 because renderer uses AGRB which is 4 bytes long to represent one pixel

        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Texture *texture = nullptr;

};

