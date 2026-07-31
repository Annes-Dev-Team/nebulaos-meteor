#include "raylib/raylib.h"
#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int w;
    int h;
    RenderTexture2D fb;

    bool isopen;
    bool allow_resizing;
} Window;

void Window_draw(Window* win);
