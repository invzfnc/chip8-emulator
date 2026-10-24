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

            if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                bool is_pressed = (event.type == SDL_EVENT_KEY_DOWN);

                switch (event.key.key) {
                    // 1 2 3 C -> 1 2 3 4
                    case SDLK_1: chip8.keys[0x1] = is_pressed; break;
                    case SDLK_2: chip8.keys[0x2] = is_pressed; break;
                    case SDLK_3: chip8.keys[0x3] = is_pressed; break;
                    case SDLK_4: chip8.keys[0xC] = is_pressed; break;

                    // 4 5 6 D -> Q W E R
                    case SDLK_Q: chip8.keys[0x4] = is_pressed; break;
                    case SDLK_W: chip8.keys[0x5] = is_pressed; break;
                    case SDLK_E: chip8.keys[0x6] = is_pressed; break;
                    case SDLK_R: chip8.keys[0xD] = is_pressed; break;

                    // 7 8 9 E -> A S D F
                    case SDLK_A: chip8.keys[0x7] = is_pressed; break;
                    case SDLK_S: chip8.keys[0x8] = is_pressed; break;
                    case SDLK_D: chip8.keys[0x9] = is_pressed; break;
                    case SDLK_F: chip8.keys[0xE] = is_pressed; break;

                    // A 0 B F -> Z X C V
                    case SDLK_Z: chip8.keys[0xA] = is_pressed; break;
                    case SDLK_X: chip8.keys[0x0] = is_pressed; break;
                    case SDLK_C: chip8.keys[0xB] = is_pressed; break;
                    case SDLK_V: chip8.keys[0xF] = is_pressed; break;
                    
                    // escape key to quit
                    case SDLK_ESCAPE:
                        if (is_pressed) running = false;
                        break;
                }
            }
        }

        for (int i = 0; i < cycles_per_frame; i++) {
            chip8.cycle();
        }

        // update screen
        platform.update(chip8.screen);

        // update timers
        chip8.delay_timer--;
        chip8.sound_timer--;

        // throttle so it does not go insanely fast
        unsigned int frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < frame_delay)
            SDL_Delay(frame_delay - frame_time);
    }
}

