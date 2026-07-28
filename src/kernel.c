#include "kernel.h"
#include "raylib/raylib.h"

const char* Folder_get_absolute_path(Folder* folder) {
    if (!folder->parent) {
        return TextFormat("/%s", folder->name);
    }
    return TextFormat("%s/%s", Folder_get_absolute_path(folder->parent), folder->name);
}

const char* File_get_absolute_path(File* file) {
    if (!file->parent) {
        return TextFormat("/%s.%s", file->name, file->ext);
    }
    return TextFormat("%s/%s.%s", Folder_get_absolute_path(file->parent), file->name, file->ext);
}
