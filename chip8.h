#pragma once
#include <string>

class Chip8 {
    public:
        Chip8();

        static constexpr int SCREEN_WIDTH = 64;
        static constexpr int SCREEN_HEIGHT = 32;
        static constexpr int MEMORY_SIZE = 4096;
        static constexpr int REGISTER_SIZE = 16;
        static constexpr int STACK_SIZE = 16;
        static constexpr int KEYS_TOTAL = 16;

        unsigned char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
        unsigned char keys[KEYS_TOTAL];

        void init();
        void cycle();

        bool load_rom(const std::string &path);

    private:
        unsigned short opcode;
        unsigned char memory[MEMORY_SIZE];
        unsigned char registers[REGISTER_SIZE];
        unsigned short index_register;
        unsigned short program_counter;

        unsigned char delay_timer;
        unsigned char sound_timer;

        unsigned short stack[STACK_SIZE];
        unsigned short stack_pointer;
};

