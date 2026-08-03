#include "nebin.h"
#include <stdint.h>
#include <stdio.h>

static uint32_t read_u32(Program *program)
{
    uint32_t value =
        (uint32_t)program->rom[program->pc] |
        ((uint32_t)program->rom[program->pc + 1] << 8) |
        ((uint32_t)program->rom[program->pc + 2] << 16) |
        ((uint32_t)program->rom[program->pc + 3] << 24);

    program->pc += 4;
    return value;
}

bool Program_step(Program *program) {
    if (program->pc >= program->rom_size) {
        return false;
    }
    uint8_t opcode = program->rom[program->pc];
    program->pc++;
    uint32_t val1 = read_u32(program);
    uint32_t val2 = read_u32(program);

    switch (opcode) {

        case OP_MOV: { // MOV SRC DEST

            if (val1 >= 4096 || val2 >= 4096) {
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
