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

    program->regs[REG_NEBOS_VERSION] = NEB_VERSION;
}

bool Program_step(Program *program) {
    if (program->pc >= program->rom_size)
        return false;
    
    uint8_t opcode = program->rom[program->pc];
    program->pc++;

    uint32_t val1 = read_u32(program);
    uint32_t val2 = read_u32(program);

    printf("OPCODE: %u\n", opcode);
    printf("VAL1: %u\n", val1);
    printf("VAL2: %u\n", val2);

    switch (opcode) {
        
        case OP_NOP:
            break;
        
        case OP_JMP:
            program->pc = val1;
            break;

        case OP_MOV: { // MOV SRCREG DSTREG
            if (val1 >= REG_COUNT || val2 >= REG_COUNT) {
                printf("Invalid register index.\n");
                return false;
            }

            program->regs[val2] = program->regs[val1];
            break;
        }

        case OP_ALLOC: // ALC SZE REG
            program->regs[val2] = malloc(val1);
            break;
        
        case OP_FREE: // FRE REG
            if (val1 < REG_FIRST_GENERAL) {
                printf("Cannot free Register %p as it is a system register\n", val1);
                return false;
            }
            free(program->regs[val1]);
            break;
        
        case OP_MOVI: // MOVI NUM REG
            *(int*)program->regs[val2] = val1;
            break;

        case OP_MOVP: { // MOVP PTRVALHGH PTRVALLOW REG

            uint32_t val3 = read_u32(program);

            uint64_t ptr_value =
                ((uint64_t)val1 << 32) |
                (uint64_t)val2;

            program->regs[val3] = (void*)ptr_value;
            break;
        }

        case OP_OSFLAG: // OSF FLAG VALUE
            break;
        
        case OP_PUTPIXEL: // PPX X Y
            break;
        
        case OP_RECTANGLE: // REC X Y W H
            break;
        
        case OP_CIRCLE: // CIC X Y RAD
            break;
            
        default:
            printf("Invalid instruction: %u\n", opcode);
            return false;
    }

    return true;
}
