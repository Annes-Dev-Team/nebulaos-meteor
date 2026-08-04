#include "raylib/raylib.h"
#include <raylib/raygui.h>
#include "window.h"
#include "config.h"
#include "settings.h"

Window settingswindow = {100, 100, 300, 500, .allow_resizing=false, .isopen=true, .draw_call=draw_settings, .decorated=true};

void init_settings() {
    settingswindow.fb = LoadRenderTexture(300, 500);
}

void draw_settings(Window* win) {
    BeginTextureMode(win->fb);
    ClearBackground(RAYWHITE);
    EndTextureMode();

    GuiSlider((Rectangle){win->x + 100, win->y, 100, 50}, "Cursor Size", "", &cursize, 0.2, 1);
}
