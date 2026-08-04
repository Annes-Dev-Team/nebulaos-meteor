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
        OP_MOV,
        0x00, 0x00, 0x00, REG_FIRST_GENERAL,   // RFIRST
        0x00, 0x00, 0x00, 0x01,   // R1

        OP_NOP,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    Program prg;
    Program_init(&prg);

    prg.rom = rom;
    prg.rom_size = sizeof(rom);

    prg.regs[REG_FIRST_GENERAL] = malloc(sizeof(int));

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
