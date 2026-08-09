#include "kernel.h"
#include "window.h"
#include <stdbool.h>

void welcomewindow_draw(Window* win) {

}

Window welcomewindow = {
    50,50,100,100, .isopen=true, true, true, "Welcome", welcomewindow_draw
};
