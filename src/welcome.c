#include "kernel.h"
#include "raylib/raylib.h"
#include "window.h"
#include <stdbool.h>
#include <stdio.h>

void welcomewindow_draw(Window* win) {
    //win->w = GetScreenWidth() - (win->x + 50);
    //win->h = GetScreenWidth() - (win->y + 50);
    //printf("%i, %i, %i, %i\n", win->x, win->y, win->w, win->h);
    BeginTextureMode(win->fb);
    ClearBackground(LIGHTGRAY);
    DrawText("Welcome to NebulaOS Meteor!", 10, 10, 30, BLACK);
    EndTextureMode();
}

Window welcomewindow = {
    100,100,500,300, .isopen=true, false, true, "Welcome", welcomewindow_draw
};

void draw_welcome() {
    Window_draw(&welcomewindow);
}

void init_welcome() {
    welcomewindow.fb = LoadRenderTexture(500, 300);
}
