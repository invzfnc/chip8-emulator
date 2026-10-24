#include "chip8.h"
#include "platform.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 640

int main() {
    Chip8 chip8;
    Platform platform { "chip8", WINDOW_WIDTH, WINDOW_HEIGHT };

    bool res = chip8.load_rom("bin/chip8-logo.ch8");

    if (res) {
        for (int i = 0; i < 20; i++) {
            chip8.cycle();
        }
    }

    platform.update(chip8.screen);

    SDL_Delay(10000);
}
