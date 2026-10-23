#include "chip8.h"
#include <iostream>

int main() {
    Chip8 chip8;

    bool res = chip8.load_rom("bin/chip8-logo.ch8");
    std::cout << res << '\n';
}
