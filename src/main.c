#include <stdlib.h>
#include <raylib/raylib.h>
#include <stdio.h>
#include "bundle.h"
#include "kernel.h"
#include "cursor.h"
#include "window.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "NebulaOS Meteor");

    kernel_init();
    init_bundle();
    HideCursor();

    load_fs();

    Window ere = {
        100, 100, 600, 600, LoadRenderTexture(600, 600)
    };

    BeginTextureMode(ere.fb);
    ClearBackground(PURPLE);
    DrawTexture(bundle_logo, 0, 0, WHITE);
    EndTextureMode();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(Folder_get_absolute_path(get_folder_by_path("/poopy")), 0, 0, 20, BLACK);
        Window_draw(&ere);
        draw_cursor();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
