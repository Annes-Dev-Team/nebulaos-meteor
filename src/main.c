#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib/raylib.h>
#include <stdio.h>
#include "bundle.h"
#include "kernel.h"
#include "cursor.h"
#include "window.h"
#include "nebin.h"
#include "settings.h"
#include "config.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "NebulaOS Meteor");
    SetTargetFPS(fps);

    kernel_init();
    init_bundle();
    init_settings();
    HideCursor();

    load_fs();

    uint8_t rom[] = {

        // ALLOC 4, R15
        // Allocate 4 bytes for an int
        OP_ALLOC,
        0x00, 0x00, 0x00, 0x04,   // size = 4
        0x00, 0x00, 0x00, 0x0F,   // R15


        // MOVI 42, R15
        // *R15 = 42
        OP_MOVI,
        0x00, 0x00, 0x00, 0x2A,   // value = 42
        0x00, 0x00, 0x00, 0x0F,   // R15


        // MOV R15, R1
        // R1 now points to the same memory
        OP_MOV,
        0x00, 0x00, 0x00, 0x0F,   // source R15
        0x00, 0x00, 0x00, 0x01,   // destination R1


        // FREE R15
        OP_FREE,
        0x00, 0x00, 0x00, 0x0F,   // R15
        0x00, 0x00, 0x00, 0x00,

        OP_MOVP,
        0x00, 0x00, 0x00, 0x00,   // 0
        0x00, 0x00, 0x00, 0x00,   // 0
        0x00, 0x00, 0x00, 0x0F,   // R15

        // NOP
        OP_NOP,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    Program prg;
    Program_init(&prg);

    prg.rom = rom;
    prg.rom_size = sizeof(rom);

    settingswindow.x = 500;

    while (!WindowShouldClose()) {
        if (Program_step(&prg)) {
            printf("PC: %u\n", prg.pc);
            printf("R1: %p\n", prg.regs[1]);
            printf("RFIRST: %p\n", prg.regs[REG_FIRST_GENERAL]);
            fflush(stdout);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(Folder_get_absolute_path(get_folder_by_path("/poopy")), 0, 0, 20, BLACK);

        Window_draw(&settingswindow);
        draw_cursor();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
