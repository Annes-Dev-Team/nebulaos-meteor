#include <raylib/raylib.h>
#include <stdio.h>
#include "bundle.h"
#include "kernel.h"
#include "cursor.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "NebulaOS Meteor");

    kernel_init();
    init_bundle();
    HideCursor();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_cursor();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
