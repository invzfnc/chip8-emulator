#include "chip8.h"
#include <cstdio>
#include <string>

unsigned char fontset[80] =
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void Chip8::init() {
    program_counter = 0x200;
    opcode = 0;
    index_register = 0;
    stack_pointer = 0;

    // clear display
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            screen[y][x] = 0;
        }
    }

    // clear stack
    for (int i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;

    // clear keys
    for (int i = 0; i < KEYS_TOTAL; i++)
        key[i] = 0;

    // clear memory
    for (int i = 0; i < MEMORY_SIZE; i++)
        memory[i] = 0;

    // clear registers 
    for (int i = 0; i < REGISTER_SIZE; i++)
        registers[i] = 0;

    // load fontset
    for (int i = 0; i < 80; i++)
        memory[i] = fontset[i];

    delay_timer = 0;
    sound_timer = 0;
}

void Chip8::cycle() {
    // fetch 2 bytes opcode
    opcode = memory[program_counter] << 8 | memory[program_counter+1];

    // decode and execute opcode
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                /* CLS: clear screen */
                case 0x00E0:
                    for (int y = 0; y < SCREEN_HEIGHT; y++)
                        for (int x = 0; x < SCREEN_WIDTH; x++)
                            screen[y][x] = 0;
                    program_counter += 2;
                    break;
                /* RET: return from subroutine */
                case 0x00EE:
                    stack_pointer--;
                    program_counter = stack[stack_pointer];
                    program_counter += 2;
                    break;
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
                    program_counter += 2;
                /* SYS addr - 0nnn is ignored */
            }
            break;
        /* JP addr (1NNN): jump to address NNN */
        case 0x1000:
            program_counter = opcode & 0x0FFF;
            break;

        /* CALL addr (2NNN): call subroutine at NNN */
        case 0x2000:
            stack[stack_pointer] = program_counter;
            stack_pointer++;
            program_counter = opcode & 0x0FFF;
            break;

        /* SE Vx, byte (3xkk): skip next instruction if Vx == kk */
        case 0x3000:
            if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                program_counter += 2;
            program_counter += 2;
            break;
    }
}

bool Chip8::load_rom(const std::string &path) {
    return true;
}

