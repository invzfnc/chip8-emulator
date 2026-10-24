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

    int pitch = 4 * Chip8::SCREEN_WIDTH;  // 4 because renderer uses AGRB which is 4 bytes long to represent one pixel
    platform.update(chip8.screen, pitch);
}
