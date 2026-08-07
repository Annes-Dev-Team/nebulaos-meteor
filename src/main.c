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

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "NebulaOS Meteor");
    SetTargetFPS(fps);

    kernel_init();
    init_bundle();
    init_settings();
    HideCursor();

    load_fs();

    /*
    uint8_t rom[] = {

        // OP_OSFLAG FLAG_WINW 640
        OP_OSFLAG,
        0x00,0x00,0x00,FLAG_WINW,
        0x00,0x00,0x02,0x80,

        // OP_OSFLAG FLAG_WINH 480
        OP_OSFLAG,
        0x00,0x00,0x00,FLAG_WINH,
        0x00,0x00,0x01,0xE0,

        // OP_OSFLAG FLAG_WINTITLE NEBVERSION
        OP_OSFLAG,
        0x00,0x00,0x00,FLAG_WINTITLE,
        0x00,0x00,0x00,REG_NEBOS_VERSION,

        // OP_INITBUF 640 480
        OP_INITBUF,
        0x00,0x00,0x02,0x80,
        0x00,0x00,0x01,0xE0,

        // RECTANGLE x=50 y=50
        OP_RECTANGLE,

        0x00,0x00,0x00,0x32, // x
        0x00,0x00,0x00,0x32, // y

        0x00,0x00,0x00,0xC8, // w = 200
        0x00,0x00,0x00,0x64, // h = 100

        0x00,0x00,0x00,0x00, // r
        0x00,0x00,0x00,0x00, // g
        0x00,0x00,0x00,0xFF, // b
        0x00,0x00,0x00,0xFF, // a

        // CIRCLE
        OP_CIRCLE,

        0x00,0x00,0x01,0x40, // x = 320
        0x00,0x00,0x00,0xF0, // y = 240

        0x00,0x00,0x00,0x3C, // radius = 60

        0x00,0x00,0x00,0xFF, // r
        0x00,0x00,0x00,0x00, // g
        0x00,0x00,0x00,0x00, // b
        0x00,0x00,0x00,0xFF, // a
    };*/
    import_to_file(get_resource("programs/test.neb"), "test", "neb");

    uint8_t* rom = files[0]->contents;

    FILE* f;
    f = fopen(get_resource("program.neb"), "wb");
    fwrite(rom, sizeof(uint8_t), files[0]->size, f);
    fclose(f);

    Program prg;
    Program_init(&prg);

    prg.rom = rom;
    prg.rom_size = files[0]->size;

    prg.win.isopen = true;
    prg.win.decorated = true;
    prg.win.allow_resizing=true;

    prg.win.x = 100;
    prg.win.y = 100;

    settingswindow.x = 500;

    Image poopy = LoadImageFromTexture(bundle_logo);
    set_window_icon(&poopy);
    //UnloadImage(poopy);

    while (!WindowShouldClose()) {
        if (Program_step(&prg)) {
            printf("PC: %u\n", prg.pc);
            printf("R1: %p\n", prg.regs[1]);
            printf("RFIRST: %p\n", prg.regs[REG_FIRST_GENERAL]);
            fflush(stdout);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        Window_draw(&settingswindow);
        Window_draw(&prg.win);
        draw_cursor();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
