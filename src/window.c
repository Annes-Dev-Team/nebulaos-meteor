#include "window.h"
#include "raylib/raylib.h"
#include <raylib/raygui.h>

void _dummy(Window* win) {}

void Window_draw(Window* win) {
    if (!win->isopen) {
        return;
    }

    if (win->decorated) {
        DrawRectangle(win->x - 10, win->y, win->w + 20, win->h + 10, GRAY); // frame
        DrawRectangle(win->x - 10, win->y - 40, win->w + 20, 40, LIGHTGRAY); // top

        DrawText(win->title, win->x, win->y - 30, 20, BLACK);

        if (GuiButton((Rectangle){win->x + win->w - 40, win->y - 40, 50, 40}, "x")) { // close button
            win->isopen = false;
        }
    }

    Rectangle src = {
        0, 0, win->w, -win->h
    };
    DrawTextureRec(win->fb.texture, src, (Vector2){win->x, win->y}, WHITE);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        // resizing
        if (win->allow_resizing) {
            if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){win->x + win->w + 10, win->y + win->h + 10}, 15)) {
                win->w += GetMouseDelta().x;
                win->h += GetMouseDelta().y;
            }
        }
        // moving
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){win->x - 10, win->y - 40, win->w + 20, 40,})) {
            win->x += GetMouseDelta().x;
            win->y += GetMouseDelta().y;
        }
    }

    BeginTextureMode(win->fb);
    DrawLine(win->fb.texture.width, 0, win->fb.texture.width, win->fb.texture.height, BLACK);
    DrawLine(0, win->fb.texture.height, win->fb.texture.width, win->fb.texture.height, BLACK);
    EndTextureMode();

    if (!win->draw_call) {
        return;
    }

    win->draw_call(win);
}
