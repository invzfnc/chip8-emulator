#include "chip8.h"
#include "platform.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <iostream>
#include <string>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 640


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <rom path> [clock speed=500]\n";
        return 1;
    }

    int clock_speed = 500;  // cycles per second

    if (argc >= 3) {
        clock_speed = std::stoi(argv[2]);
    }

    constexpr int target_fps = 60;    // frames per second
    const int cycles_per_frame = clock_speed / target_fps;
    const unsigned int frame_delay = 1000 / target_fps;  // in ms

    Chip8 chip8;
    Platform platform { "cha's chip8 emulator                     ", WINDOW_WIDTH, WINDOW_HEIGHT };

    bool res = chip8.load_rom(argv[1]);

    if (!res) {
        std::cout << "Can't load rom\n";
        return 1;
    }

    bool running = true;
    SDL_Event event;


    while (running) {
        unsigned int frame_start = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        for (int i = 0; i < cycles_per_frame; i++) {
            chip8.cycle();
        }

        // update screen
        platform.update(chip8.screen);

        // throttle so it does not go insanely fast
        unsigned int frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < frame_delay)
            SDL_Delay(frame_delay - frame_time);
    }
}

