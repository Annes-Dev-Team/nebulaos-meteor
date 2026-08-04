#include "nebin.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t read_u32(Program *program) // reads in big endian
{
    uint32_t value =
        ((uint32_t)program->rom[program->pc] << 24) |
        ((uint32_t)program->rom[program->pc + 1] << 16) |
        ((uint32_t)program->rom[program->pc + 2] << 8) |
        (uint32_t)program->rom[program->pc + 3];

    program->pc += 4;
    return value;
}

void Program_init(Program *program) {
    memset(program, 0, sizeof(Program));
    
    program->regs[REG_WINDOW_HEIGHT] = malloc(sizeof(int));
    program->regs[REG_WINDOW_FPS] = malloc(sizeof(int));
    program->regs[REG_WINDOW_WIDTH] = malloc(sizeof(int));

    program->regs[REG_NEBOS_VERSION] = NEB_VERSION;

    program->regs[REG_WINDOW_RESIZABLE] = malloc(sizeof(bool));
}

bool Program_step(Program *program) {
    if (program->pc >= program->rom_size)
        return false;
    
    uint8_t opcode = program->rom[program->pc];
    program->pc++;

    uint32_t val1 = read_u32(program);
    uint32_t val2 = read_u32(program);

    switch (opcode) {
        
        case OP_NOP:
            break;
        
        case OP_JMP:
            program->pc = val1;
            break;

        case OP_MOV: { // MOV SRC DEST

            if (val1 >= REG_COUNT || val2 >= REG_COUNT) {
                printf("Invalid register index.\n");
                return false;
            }

            program->regs[val2] = program->regs[val1];
            break;
        }

        default:
            printf("Invalid instruction: %u\n", opcode);
            return false;
    }

    return true;
}
