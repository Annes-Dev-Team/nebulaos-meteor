#include "raylib/raylib.h"
#include "window.h"
#include <stdint.h>

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

    REG_WINDOW_WIDTH,
    REG_WINDOW_HEIGHT,
    REG_WINDOW_FPS,
    REG_WINDOW_RESIZABLE,
    REG_WINDOW_OPEN,
    REG_WINDOW_DECORATED,

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
    OP_MOVP
} Opcodes;

typedef struct {
    const uint8_t* rom;
    uint32_t rom_size;
    uint32_t pc; // its unlikely anyone will make a binary bigger than 4 gigs
    void* regs[REG_COUNT]; // Regs 0-8 will be function arguments 8-12 will be window info 13 will be nebos version.
    Window win;
} Program;

void Program_init(Program *program);
bool Program_step(Program* program);
