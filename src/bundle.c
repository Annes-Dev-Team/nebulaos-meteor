#include "raylib/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>

#ifdef __APPLE__

#include <CoreFoundation/CoreFoundation.h> // wow apple has a fucking c library WWWWOOOOOOWWWW
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>

const char* get_resource(const char* resource) {
    static char path[PATH_MAX];

    CFBundleRef bundle = CFBundleGetMainBundle();
    if (!bundle)
        return NULL;

    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(bundle);
    if (!resourcesURL)
        return NULL;

    if (!CFURLGetFileSystemRepresentation(resourcesURL, true,
                                          (UInt8*)path, sizeof(path))) {
        CFRelease(resourcesURL);
        return NULL;
    }

    CFRelease(resourcesURL);

    size_t len = strlen(path);
    snprintf(path + len, sizeof(path) - len, "/%s", resource);

    return path;
}

const char *get_save_path(void) {
    static char path[1024];

    const char *home = getenv("HOME");
    snprintf(path, sizeof(path),
             "%s/Library/Application Support/NebMeteor",
             home);

    // Create the directory if it doesn't exist.
    mkdir(path, 0755);

    strcat(path, "/filesystem.json");
    return path;
}

#else 

const char* get_resource(const char *resource)
{
    return TextFormat("%s%s",
                      GetApplicationDirectory(),
                      resource);
}

const char *get_save_path(void) {
    return "filesystem.json";
}

#endif

Texture2D bundle_logo;
Texture2D bundle_arrow;
Texture2D bundle_toby;
Texture2D bundle_cursor;
Texture2D bundle_curhover;

Texture2D bundle_unfile;
Texture2D bundle_imgfile;
Texture2D bundle_soundfile;

void init_bundle(void) {
    bundle_arrow = LoadTexture(get_resource("arrow.png"));
    bundle_logo = LoadTexture(get_resource("logo.png"));
    bundle_cursor = LoadTexture(get_resource("cursor.png"));
    bundle_curhover = LoadTexture(get_resource("arrow.png"));
    bundle_toby = LoadTexture(get_resource("arrow.png"));
    
    bundle_unfile = LoadTexture(get_resource("arrow.png"));
    bundle_imgfile = LoadTexture(get_resource("arrow.png"));
    bundle_soundfile = LoadTexture(get_resource("arrow.png"));
    
}

