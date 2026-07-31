#include "raylib/raylib.h"

typedef struct {
    int x;
    int y;
    int w;
    int h;
    RenderTexture2D fb;

} Window;

void Window_draw(Window* win);
