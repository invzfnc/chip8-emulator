#include "chip8.h"
#include "platform.h"
#include <iostream>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

int main() {
    Chip8 chip8;
    Platform platform { "cha's Chip8 Emulator", WINDOW_WIDTH, WINDOW_HEIGHT };

    bool res = chip8.load_rom("bin/chip8-logo.ch8");
    std::cout << res << '\n';

    if (res) {
        for (int i = 0; i < 20; i++) {
            chip8.cycle();
        }
    }

    platform.update(chip8.screen);

    SDL_Delay(10000);
}
