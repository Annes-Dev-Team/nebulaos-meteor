#include "raylib/raylib.h"
#include <stdbool.h>
#pragma once

typedef struct Window Window;

typedef struct Window {
    int x;
    int y;
    int w;
    int h;
    RenderTexture2D fb;

    bool isopen;
    bool allow_resizing;
    bool decorated;

    const char* title;

    void (*draw_call)(Window*);
} Window;

void _dummy(Window* win);

void Window_draw(Window* win);
