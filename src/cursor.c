#include "raylib/raylib.h"
#include <raylib/raygui.h>
#include "bundle.h"
#include <stdbool.h>
#include <stdlib.h>

bool showmenu = 0;
Vector2 savedmousepos;

void draw_cursor() {
    if (showmenu) {
        Rectangle ere = {savedmousepos.x, savedmousepos.y, 100, 50};
       if (GuiButton(ere, "Exit")) {
            exit(0);
       }
    }
    DrawTextureEx(bundle_cursor, GetMousePosition(), 0, 0.4, WHITE);
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        savedmousepos = GetMousePosition();
        showmenu = !showmenu;
    }
}
