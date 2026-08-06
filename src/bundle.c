#include "raylib/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>

#ifdef __APPLE__

#include <CoreFoundation/CoreFoundation.h> // wow apple has a fucking c library WWWWOOOOOOWWWW
#include <CoreFoundation/CFBase.h>

#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>

#include <objc/objc.h>
#include <objc/runtime.h>
#include <objc/message.h>

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

static void set_app_icon(id nsImage)
{
    Class NSApplication = (Class)objc_getClass("NSApplication");

    id app = ((id (*)(Class, SEL))objc_msgSend)(
        NSApplication,
        sel_registerName("sharedApplication"));

    ((void (*)(id, SEL, id))objc_msgSend)(
        app,
        sel_registerName("setApplicationIconImage:"),
        nsImage);
}

#else 

const char* get_resource(const char *resource)
{
    return TextFormat("%s%s",
                      GetApplicationDirectory(),
                      resource);
}

const char *get_save_path(void) {
    return "nebfs.json";
}

#endif
#include <time.h>

Texture2D bundle_logo;
Texture2D bundle_arrow;
Texture2D bundle_toby;
Texture2D bundle_cursor;
Texture2D bundle_curhover;

Texture2D bundle_unfile;
Texture2D bundle_imgfile;
Texture2D bundle_soundfile;

void init_bundle(void) {
    time_t raw_time;
    time(&raw_time);
    struct tm *local_time = localtime(&raw_time);
    unsigned char current_month = local_time->tm_mon + 1;

    bundle_arrow = LoadTexture(get_resource("arrow.png"));
    if (current_month == 6) { // June
        bundle_logo = LoadTexture(get_resource("pride.png"));
    } else {
        bundle_logo = LoadTexture(get_resource("logo.png"));
    }
    bundle_cursor = LoadTexture(get_resource("cursor.png"));
    bundle_curhover = LoadTexture(get_resource("arrow.png"));
    bundle_toby = LoadTexture(get_resource("arrow.png"));
    
    bundle_unfile = LoadTexture(get_resource("arrow.png"));
    bundle_imgfile = LoadTexture(get_resource("arrow.png"));
    bundle_soundfile = LoadTexture(get_resource("arrow.png"));
    
}

void set_window_icon(Image *image)
{
#ifdef __APPLE__
    if (!image || !image->data) return;

    // Convert to RGBA8 if necessary
    Image rgba = *image;
    if (rgba.format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8)
        ImageFormat(&rgba, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    Class NSBitmapImageRep = (Class)objc_getClass("NSBitmapImageRep");
    Class NSImageClass     = (Class)objc_getClass("NSImage");
    Class NSStringClass    = (Class)objc_getClass("NSString");

    // NSString *colorSpace = @"NSCalibratedRGBColorSpace";
    id colorSpace = ((id (*)(Class, SEL, const char *))objc_msgSend)(
        NSStringClass,
        sel_registerName("stringWithUTF8String:"),
        "NSCalibratedRGBColorSpace");

    // [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:...]
    id bitmap = ((id (*)(id, SEL, void *,
                         CFIndex, CFIndex,
                         CFIndex, CFIndex,
                         BOOL, BOOL,
                         id,
                         CFIndex, CFIndex))objc_msgSend)(
        ((id (*)(Class, SEL))objc_msgSend)(
            NSBitmapImageRep,
            sel_registerName("alloc")),
        sel_registerName("initWithBitmapDataPlanes:pixelsWide:pixelsHigh:bitsPerSample:samplesPerPixel:hasAlpha:isPlanar:colorSpaceName:bytesPerRow:bitsPerPixel:"),
        NULL,
        rgba.width,
        rgba.height,
        8,
        4,
        YES,
        NO,
        colorSpace,
        rgba.width * 4,
        32);

    unsigned char *dst =
        ((unsigned char *(*)(id, SEL))objc_msgSend)(
            bitmap,
            sel_registerName("bitmapData"));

    memcpy(dst, rgba.data, rgba.width * rgba.height * 4);

    typedef struct {
        double width;
        double height;
    } NSSize;

    NSSize size = { rgba.width, rgba.height };

    id nsImage = ((id (*)(id, SEL, NSSize))objc_msgSend)(
        ((id (*)(Class, SEL))objc_msgSend)(
            NSImageClass,
            sel_registerName("alloc")),
        sel_registerName("initWithSize:"),
        size);

    ((void (*)(id, SEL, id))objc_msgSend)(
        nsImage,
        sel_registerName("addRepresentation:"),
        bitmap);

    set_app_icon(nsImage);

    ((void (*)(id, SEL))objc_msgSend)(bitmap, sel_registerName("release"));
    ((void (*)(id, SEL))objc_msgSend)(nsImage, sel_registerName("release"));

    if (&rgba != image)
        UnloadImage(rgba);

#else
    SetWindowIcon(*image);
#endif
}
