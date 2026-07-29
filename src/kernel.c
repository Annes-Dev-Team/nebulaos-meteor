#include "kernel.h"
#include "raylib/raylib.h"
#include <stdlib.h>
#include <string.h>

Folder** folders = NULL;
File** files = NULL;

size_t folder_count=0;
size_t file_count = 0;

void kernel_init(void) {
    folder_count = 0;
    folders = malloc(sizeof(Folder*) * folder_count);

    file_count = 0;
    files = malloc(sizeof(File*) * folder_count);
}

const char* Folder_get_absolute_path(Folder* folder) {
    if (!folder->parent) {
        return TextFormat("/%s", folder->name);
    }
    return TextFormat("%s/%s", Folder_get_absolute_path(folder->parent), folder->name);
}

Folder* get_folder_by_path(const char* path) {
    for (size_t i = 0; i < folder_count; i++) {
        if (strcmp(Folder_get_absolute_path(folders[i]), path) == 0)
            return folders[i];
    }
    return NULL;
}

void Folder_add(Folder* folder) {
    folders = realloc(folders, sizeof(Folder*) * (folder_count + 1));
    folders[folder_count++] = folder;
}

const char* File_get_absolute_path(File* file) {
    if (!file->parent) {
        return TextFormat("/%s.%s", file->name, file->ext);
    }
    return TextFormat("%s/%s.%s", Folder_get_absolute_path(file->parent), file->name, file->ext);
}

void File_add(File* file) {
    files = realloc(files, sizeof(File*) * (file_count + 1));
    files[file_count++] = file;
}

File* get_file_by_path(const char* path) {
    for (size_t i = 0; i < file_count; i++) {
        if (strcmp(File_get_absolute_path(files[i]), path) == 0)
            return files[i];
    }
    return NULL;
}
