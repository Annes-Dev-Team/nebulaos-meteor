#include <stdint.h>
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
#include "fsextra.h"
#include "welcome.h"

#define INSTRUCTIONS_PER_FRAME 20

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "NebulaOS Meteor");
    SetTargetFPS(fps);

    kernel_init();

    init_bundle();
    init_settings();
    init_welcome();

    HideCursor();

    load_fs();
    
    import_to_file(get_resource("programs/starcatcher.neb"), "test", "neb");

    uint8_t* rom = files[0]->contents;

    Program prg;
    Program_init(&prg);

    prg.rom = rom;
    prg.rom_size = files[0]->size;

    prg.win.decorated = true;
    prg.win.allow_resizing=true;

    prg.win.x = 100;
    prg.win.y = 100;

    settingswindow.x = 500;

    Image poopy = LoadImageFromTexture(bundle_logo);
    set_window_icon(&poopy);
    //UnloadImage(poopy);

    while (!WindowShouldClose()) {
        for (int i = 0; i < INSTRUCTIONS_PER_FRAME; i++) {
            if (Program_step(&prg)) {
                printf("PC: %u\n", prg.pc);
                printf("RFIRST: %p\n", prg.regs[REG_FIRST_GENERAL]);
                printf("R9: %p\n", prg.regs[REG_KEYINPUT]);
                fflush(stdout);
            } else {
                break;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        Window_draw(&settingswindow);
        Window_draw(&prg.win);
        //draw_welcome();
        
        draw_cursor();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
