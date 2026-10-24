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

void Platform::update(unsigned char buffer[][Chip8::SCREEN_WIDTH], int pitch) {
    // put screen buffer onto texture
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);

    // set the draw color to black (abgr)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    // paint the whole screen black
    SDL_RenderClear(renderer);

    // put texture onto renderer, auto scale it to fill the whole screen
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
