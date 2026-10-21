#include <string>

class Chip8 {
    public:
        Chip8();

        static constexpr int SCREEN_WIDTH = 64;
        static constexpr int SCREEN_HEIGHT = 32;
        static constexpr int MEMORY = 4096;
        static constexpr int REGISTERS = 16;
        static constexpr int STACK_SIZE = 16;
        static constexpr int KEYS = 16;

        void init();
        void cycle();

        bool load_rom(const std::string &path);

    private:
        unsigned short opcode;
        unsigned char memory[MEMORY];
        unsigned char registers[REGISTERS];
        unsigned short index_register;
        unsigned short program_counter;

        unsigned char screen[SCREEN_HEIGHT][SCREEN_WIDTH];

        unsigned char delay_timer;
        unsigned char sound_timer;

        unsigned short stack[STACK_SIZE];
        unsigned short stack_pointer;

        unsigned char key[KEYS];

};
