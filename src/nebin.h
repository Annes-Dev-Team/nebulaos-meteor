#include "raylib/raylib.h"
#include <stdint.h>

typedef enum {
    REG_ARG0 = 0,
    REG_ARG1,
    REG_ARG2,
    REG_ARG3,
    REG_ARG4,
    REG_ARG5,
    REG_ARG6,
    REG_ARG7,

    REG_WINDOW_WIDTH,
    REG_WINDOW_HEIGHT,
    REG_WINDOW_FPS,
    REG_WINDOW_FLAGS,
    REG_WINDOW_HANDLE,

    REG_NEBOS_VERSION,

    REG_FIRST_GENERAL
} SpecialRegisters;

typedef enum {
    OP_NOP = 0,
    OP_MOV,
    OP_JMP
} Opcodes;

typedef struct {
    const uint8_t* rom;
    uint32_t rom_size;
    uint32_t pc; // its unlikely anyone will make a binary bigger than 4 gigs
    void* regs[4096]; // Regs 0-8 will be function arguments 8-12 will be window info 13 will be nebos version.
    RenderTexture2D screen;

} Program;

bool Program_step(Program* program);
