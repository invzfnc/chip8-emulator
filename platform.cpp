#include "platform.h"
#include "chip8.h"
#include <SDL3/SDL_render.h>

Platform::Platform(const char *title, int window_width, int window_height) {
    if (!SDL_Init(SDL_INIT_VIDEO))  {
        SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow(title, window_width, window_height, 0);  // TODO: try resizable
    renderer = SDL_CreateRenderer(window, nullptr);
    texture = SDL_CreateTexture(renderer, 
                                SDL_PIXELFORMAT_ABGR8888,      // alpha, blue, green, red, each 8 bits, 32 bits in total for each pixel
                                SDL_TEXTUREACCESS_STREAMING,   // https://wiki.libsdl.org/SDL3/SDL_TextureAccess
                                Chip8::SCREEN_WIDTH,
                                Chip8::SCREEN_HEIGHT);
}

Platform::~Platform() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::update(unsigned char screen[][Chip8::SCREEN_WIDTH]) {
    // put screen buffer onto texture
    unsigned int buffer[Chip8::SCREEN_HEIGHT][Chip8::SCREEN_WIDTH];

    // convert raw buffer to match texture grid
    for (int y = 0; y < Chip8::SCREEN_HEIGHT; y++) {
        for (int x = 0; x < Chip8::SCREEN_WIDTH; x++) {
            buffer[y][x] = (screen[y][x] ? 0xFFFFFFFF : 0xFF000000);
        }
    }

    SDL_UpdateTexture(texture, nullptr, buffer, pitch);

    // set the draw color to black (rgba)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // paint the whole screen black
    SDL_RenderClear(renderer);

    // put texture onto renderer, auto scale it to fill the whole screen
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
