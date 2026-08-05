#include "nebin.h"
#include "config.h"
#include "raylib/raylib.h"
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
            switch (val1) {
                case FLAG_WINOPEN:
                    program->win.isopen = val2;
                    break;

                case FLAG_WINW:
                    program->win.w = val2;
                    break;
                
                case FLAG_WINH:
                    program->win.h = val2;
                    break;
                
                case FLAG_WINX:
                    program->win.x = val2;
                    break;
                
                case FLAG_WINY:
                    program->win.y = val2;
                    break;
                
                case FLAG_WINDEC:
                    program->win.decorated = val2;
                    break;

            }
            break;
        
        case OP_INITBUF:
            program->win.fb = LoadRenderTexture(val1, val2);
            break;

        case OP_PUTPIXEL: { // PPX X Y R G B A
            uint32_t r = read_u32(program);
            uint32_t g = read_u32(program);
            uint32_t b = read_u32(program);
            uint32_t a = read_u32(program);

            BeginTextureMode(program->win.fb);
            DrawPixel(val1, val2, (Color){r,g,b, a});
            EndTextureMode();
            break;
        }
        
        case OP_RECTANGLE: { // REC X Y W H R G B A
            uint32_t w = read_u32(program);
            uint32_t h = read_u32(program);

            uint32_t r = read_u32(program);
            uint32_t g = read_u32(program);
            uint32_t b = read_u32(program);
            uint32_t a = read_u32(program);

            BeginTextureMode(program->win.fb);
            DrawRectangle(val1, val2, w, h, (Color){r,g,b,a});
            EndTextureMode();
            break;
        }

        case OP_CIRCLE: {// CIC X Y RAD R G B A
            uint32_t rad = read_u32(program);

            uint32_t r = read_u32(program);
            uint32_t g = read_u32(program);
            uint32_t b = read_u32(program);
            uint32_t a = read_u32(program);

            BeginTextureMode(program->win.fb);
            DrawCircle(val1, val2, rad, (Color){r,g,b, a});
            EndTextureMode();
            break;
        }

        default:
            printf("Invalid instruction: %u\n", opcode);
            return false;
    }

    return true;
}
