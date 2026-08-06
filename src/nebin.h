#include "raylib/raylib.h"
#include "window.h"
#include <stdint.h>

#pragma once

#define REG_COUNT 4096

typedef enum {
    REG_ARG0 = 0,
    REG_ARG1,
    REG_ARG2,
    REG_ARG3,
    REG_ARG4,
    REG_ARG5,
    REG_ARG6,
    REG_ARG7,

    REG_NEBOS_VERSION,

    REG_FIRST_GENERAL
} SpecialRegisters;

typedef enum {
    OP_NOP = 0,
    OP_MOV,
    OP_JMP,

    OP_ALLOC,
    OP_FREE,
    OP_MOVI,
    OP_MOVP,

    OP_OSFLAG,

    OP_INITBUF,

    OP_PUTPIXEL,
    OP_RECTANGLE,
    OP_CIRCLE,

    OP_READFILE,
    OP_SETARRAYINDEX
} Opcodes;

typedef enum {
    FLAG_WINOPEN = 0,
    FLAG_WINW,
    FLAG_WINH,
    FLAG_WINX,
    FLAG_WINY,
    FLAG_WINDEC,

    FLAG_WINRESIZE,
    FLAG_WINTITLE
} OSFlags;

typedef struct {
    const uint8_t* rom;
    uint32_t rom_size;
    uint32_t pc; // its unlikely anyone will make a binary bigger than 4 gigs
    void* regs[REG_COUNT]; // Regs 0-7 will be function arguments 8 will be nebos version.
    Window win;
} Program;

void Program_init(Program *program);
bool Program_step(Program* program);
