#include <raylib/raylib.h>
#include <stdio.h>
#include "bundle.h"
#include "kernel.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "NebulaOS Meteor");

    init_bundle();
    Folder yeet = {0};
    yeet.name = "i like femboys";
    Folder yeet2 = {0};
    yeet2.parent = &yeet;
    yeet2.name = "yeet";
    File fad = {0};
    fad.parent = &yeet2;
    fad.name = "poop";
    fad.ext = "txt";

    puts(Folder_get_absolute_path(&yeet2));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(File_get_absolute_path(&fad), 0, 0, 20, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
