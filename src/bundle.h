#include "raylib/raylib.h"

const char* get_resource(const char* resource);

extern Texture2D bundle_logo;
extern Texture2D bundle_arrow;
extern Texture2D bundle_toby;
extern Texture2D bundle_cursor;
extern Texture2D bundle_curhover;

extern Texture2D bundle_unfile;
extern Texture2D bundle_imgfile;
extern Texture2D bundle_soundfile;

void init_bundle(void);
const char *get_save_path(void);

// MacOS is known to not comply with the GLFW function that is why this function exists
void set_window_icon(Image* image); 
