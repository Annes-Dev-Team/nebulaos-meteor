#include "raylib/raylib.h"
#include <raylib/raygui.h>
#include "window.h"
#include "config.h"

Window settingswindow = {100, 100, 300, 500, .allow_resizing=false, .isopen=true};

void init_settings() {
    settingswindow.fb = LoadRenderTexture(300, 500);
}

void draw_settings() {
    BeginTextureMode(settingswindow.fb);
    ClearBackground(RAYWHITE);
    EndTextureMode();

    Window_draw(&settingswindow);
    GuiSlider((Rectangle){settingswindow.x + 100, settingswindow.y, 100, 50}, "Cursor Size", "", &cursize, 0.2, 1);
}
