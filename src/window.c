#include "window.h"
#include "raylib/raylib.h"

void Window_draw(Window* win) {
    BeginTextureMode(win->fb);
    DrawLine(win->fb.texture.width, 0, win->fb.texture.width, win->fb.texture.height, BLACK);
    DrawLine(0, win->fb.texture.height, win->fb.texture.width, win->fb.texture.height, BLACK);
    EndTextureMode();

    DrawRectangle(win->x - 10, win->y, win->w + 20, win->h + 10, GRAY);
    DrawRectangle(win->x - 10, win->y - 40, win->w + 20, 40, LIGHTGRAY);
    Rectangle src = {
        0, 0, win->w, -win->h
    };
    DrawTextureRec(win->fb.texture, src, (Vector2){win->x, win->y}, WHITE);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        // resizing
        if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){win->x + win->w + 10, win->y + win->h + 10}, 15)) {
            win->w += GetMouseDelta().x;
            win->h += GetMouseDelta().y;
        }
        // moving
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){win->x - 10, win->y - 40, win->w + 20, 40,})) {
            win->x += GetMouseDelta().x;
            win->y += GetMouseDelta().y;
        }
    }
}
