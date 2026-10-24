#include "chip8.h"
#include "platform.h"
#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 640

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <rom path>\n";
        return 1;
    }

    Chip8 chip8;
    Platform platform { "cha's chip8 emulator                     ", WINDOW_WIDTH, WINDOW_HEIGHT };

    bool res = chip8.load_rom(argv[1]);

    if (!res) {
        std::cout << "Can't load rom\n";
        return 1;
    }

    for (int i = 0; i < 20; i++) {
        chip8.cycle();
    }

    platform.update(chip8.screen);

    SDL_Delay(5000);
}
