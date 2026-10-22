#include "chip8.h"
#include <cstdio>
#include <cstdlib>
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
        keys[i] = 0;

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

    // advance to next instruction
    program_counter += 2;

    // decode and execute opcode
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                /* CLS: clear screen */
                case 0x00E0:
                    for (int y = 0; y < SCREEN_HEIGHT; y++)
                        for (int x = 0; x < SCREEN_WIDTH; x++)
                            screen[y][x] = 0;
                    break;
                /* RET: return from subroutine */
                case 0x00EE:
                    stack_pointer--;
                    program_counter = stack[stack_pointer];
                    break;
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
                /* SYS addr - 0nnn is ignored */
            }
            break;
        /* JP addr (1Nnn): jump to address nnn */
        case 0x1000:
            program_counter = opcode & 0x0FFF;
            break;

        /* CALL addr (2nnn): call subroutine at nnn */
        case 0x2000:
            stack[stack_pointer] = program_counter;
            stack_pointer++;
            program_counter = opcode & 0x0FFF;
            break;

        /* SE Vx, byte (3xkk): skip next instruction if Vx == kk */
        case 0x3000:
            if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                program_counter += 2;
            break;

        /* SNE Vx, byte (4xkk): skip next instruction if Vx != kk */
        case 0x4000:
            if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                program_counter += 2;
            break;

        /* SE Vx, Vy (5xy0): skip next instruction if Vx == Vy */
        case 0x5000:
            if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x00F0) >> 4])
                program_counter += 2;
            break;
        /* LD Vx, byte (6xkk): load the value kk into register Vx */

        case 0x6000:
            registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            break;

        /* ADD Vx, byte (7xkk): add value kk to the register Vx */
        case 0x7000:
            registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;

        case 0x8000:
            switch (opcode & 0x000F) {
                /* ED Vx, Vy (8xy0): Vx = Vy */
                case 0x0000:
                    registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
                    break;

                /* OR Vx, Vy (8xy1): Vx = Vx OR Vy */
                case 0x0001:
                    registers[(opcode & 0x0F00) >> 8] |= registers[(opcode & 0x00F0) >> 4];
                    break;

                /* AND Vx, Vy (8xy2): Vx = Vx AND Vy */
                case 0x0002:
                    registers[(opcode & 0x0F00) >> 8] &= registers[(opcode & 0x00F0) >> 4];
                    break;

                /* XOR Vx, Vy (8xy3): Vx = Vx XOR Vy */
                case 0x0003:
                    registers[(opcode & 0x0F00) >> 8] ^= registers[(opcode & 0x00F0) >> 4];
                    break;

                /* ADD Vx, Vy (8xy4): Vx += Vy, set VF = carry */
                case 0x0004:
                    if (registers[(opcode & 0x00F0) >> 4] > 0xFF - registers[(opcode & 0x0F00) >> 8])
                        registers[0xF] = 1;
                    else
                        registers[0xF] = 0;
                    registers[(opcode & 0x0F00) >> 8] += registers[(opcode & 0x00F0) >> 4];
                    break;

                /* SUB Vx, Vy (8xy5): Vx -= Vy, set VF = NOT borrow */
                case 0x0005:
                    registers[0xF] = registers[(opcode & 0x0F00) >> 8] >= registers[(opcode & 0x00F0) >> 4];
                    registers[(opcode & 0x0F00) >> 8] -= registers[(opcode & 0x00F0) >> 4];
                    break;

                /* SHR Vx {, Vy} (8xy6): shift Vx to right by one. VF is set to the value of least significant bit of Vx before the shift*/
                case 0x0006:
                    registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 0x1;
                    registers[(opcode & 0x0F00) >> 8] >>= 1;
                    break;

                /* SUBN Vx, Vy (8xy7): Vx = Vy - Vx, set VF = NOT borrow */
                case 0x0007:
                    registers[0xF] = registers[(opcode & 0x0F00) >> 8] <= registers[(opcode & 0x00F0) >> 4];
                    registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4] - registers[(opcode & 0x0F00) >> 8];
                    break;

                /* SHL Vx {, Vy} (8xyE): shift Vx to left by one. VF is set to the value of most significant bit of Vx before the shift*/
                case 0x000E:
                    registers[0xF] = registers[(opcode & 0x0F00) >> 8] >> 7;
                    registers[(opcode & 0x0F00) >> 8] <<= 1;
                    break;

                default:
                    printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
            }
            break;

        /* SNE Vx, Vy (9xy0): skip next instruction if Vx != Vy */
        case 0x9000:
            if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4])
                program_counter += 2;
            break;

        /* LD I, addr (Annn): set register I = nnn */
        case 0xA000:
            index_register = opcode & 0x0FFF;
            break;

        /* JP V0, addr (Bnnn): jump to location nnn + V0 */
        case 0xB000:
            program_counter = (opcode & 0x0FFF) + registers[0];
            break;

        /* RND Vx, byte (Cxkk): set Vx = random byte AND kk */
        case 0xC000:
            registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & std::rand();
            break;

        /* DRW Vx, Vy, nibble (Dxyn): display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision */
        case 0xD000:
            // TODO
            break;

        case 0xE000:
            switch (opcode & 0x00FF) {
                /* SKP Vx (Ex9E): skip next instruction if key with the value of Vx is pressed */
                case 0x9E:
                    if (keys[registers[(opcode & 0x0F00) >> 8]] != 0)
                        program_counter += 2;
                    break;
                /* SKNP Vx (ExA1): skip next instruction if key with the value of Vx is not pressed */
                case 0xA1:
                    if (keys[registers[(opcode & 0x0F00) >> 8]] == 0)
                        program_counter += 2;
                    break;
                default:
                    printf("Unknown opcode [0xE000]: 0x%X\n", opcode);
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF) {
                /* LD Vx, DT (Fx07): set Vx = delay timer value */
                case 0x07:
                    registers[(opcode & 0x0F00) >> 8] = delay_timer;
                    break;

                /* LD Vx, K (Fx0A): wait for a key press, store the value of the key in Vx */
                case 0x0A: {
                    bool key_pressed = false;

                    for (int i = 0; i < KEYS_TOTAL; i++) {
                        if (keys[i] != 0) {
                            registers[(opcode & 0x0F00) >> 8] = i;
                            key_pressed = true;
                            break;
                        }
                    }

                    if (!key_pressed) {
                        program_counter -= 2;  // run this instruction again on next cycle
                        return;
                    }
                    break;
                }

                /* LD DT, Vx (Fx15): set delay timer = Vx */
                case 0x15:
                    delay_timer = registers[(opcode & 0x0F00) >> 8];
                    break;

                /* LD ST, Vx (Fx18): set sound timer = Vx */
                case 0x18:
                    sound_timer = registers[(opcode & 0x0F00) >> 8];
                    break;

                /* ADD I, Vx (Fx1E): I += Vx */
                case 0x1E:
                    index_register += registers[(opcode & 0x0F00) >> 8];
                    break;

                /* LD F, Vx (Fx29): I = location of sprite for digit Vx */
                case 0x29:
                    index_register = registers[(opcode & 0x0F00) >> 8] * 5;
                    break;

                /* LD B, Vx (Fx33): store BCD representation/decimal digits of Vx in memory I, I+1, and I+2 */
                case 0x33: {
                    const auto n = registers[(opcode & 0x0F00) >> 8];

                    memory[index_register] = n / 100;
                    memory[index_register + 1] = n / 10 % 10;
                    memory[index_register + 2] = n % 10;
                    break;
               }

                /* LD [I], Vx (Fx55): store values of V0 to VX to memory starting at address I */
                case 0x55:
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
                        memory[index_register + i] = registers[i];
                    }
                    break;

                /* LD Vx, [I] (Fx65): fill V0 to VX with values from memory starting at address I */
                case 0x65:
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
                        registers[i] = memory[index_register + i];
                    }
                    break;

                default:
                    printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
            }
            break;

    }
}

bool Chip8::load_rom(const std::string &path) {
    // TODO
    return true;
}

