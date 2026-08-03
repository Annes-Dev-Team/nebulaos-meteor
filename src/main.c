#include <stdbool.h>
#include <stdlib.h>
#include <raylib/raylib.h>
#include <stdio.h>
#include "bundle.h"
#include "kernel.h"
#include "cursor.h"
#include "window.h"
#include "settings.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "NebulaOS Meteor");

    kernel_init();
    init_bundle();
    init_settings();
    HideCursor();

    load_fs();

    while (!WindowShouldClose()) {
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
