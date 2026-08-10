#include "raylib/raylib.h"
#include <raylib/raygui.h>
#include "window.h"
#include "config.h"
#include "settings.h"

Window settingswindow = {100, 100, 300, 500, .allow_resizing=false, .isopen=false, .draw_call=draw_settings, .decorated=true, .title="Settings"};

void init_settings() {
    settingswindow.fb = LoadRenderTexture(300, 500);
}

void draw_settings(Window* win) {
    BeginTextureMode(win->fb);
    ClearBackground(RAYWHITE);
    EndTextureMode();

    GuiSlider((Rectangle){win->x + 100, win->y, 100, 50}, "Cursor Size", TextFormat("%f", cursize), &cursize, 0.2, 1);
    
    static float fps_slider = 60.0f;

    GuiSlider(
        (Rectangle){win->x + 100, win->y + 60, 100, 50},
        "FPS",
        TextFormat("%.0f", fps_slider),
        &fps_slider,
        5,
        120
    );

    fps = (int)fps_slider;

    if (fps != GetFPS()) {
        SetTargetFPS(fps);
    }
}
