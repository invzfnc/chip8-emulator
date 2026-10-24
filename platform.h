#pragma once

#include "chip8.h"
#include <SDL3/SDL.h>

class Platform {

    public:
        Platform(const char *title, int window_width, int window_height);
        ~Platform();
        void update(unsigned char buffer[][Chip8::SCREEN_WIDTH], int pitch);

    private:
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Texture *texture = nullptr;

};

