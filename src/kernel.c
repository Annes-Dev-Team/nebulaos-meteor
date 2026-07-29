#include "kernel.h"
#include "raylib/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>
#include "bundle.h"

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

void save_fs(void) {
    cJSON *root = cJSON_CreateObject();

    cJSON *folders_json = cJSON_AddArrayToObject(root, "folders");
    for (size_t i = 0; i < folder_count; i++) {
        Folder *folder = folders[i];

        cJSON *obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj, "name", folder->name);

        if (folder->parent)
            cJSON_AddStringToObject(obj, "parent", Folder_get_absolute_path(folder->parent));
        else
            cJSON_AddNullToObject(obj, "parent");

        cJSON_AddItemToArray(folders_json, obj);
    }

    cJSON *files_json = cJSON_AddArrayToObject(root, "files");
    for (size_t i = 0; i < file_count; i++) {
        File *file = files[i];

        cJSON *obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj, "name", file->name);
        cJSON_AddStringToObject(obj, "ext", file->ext);
        cJSON_AddStringToObject(obj, "contents", file->contents);

        if (file->parent)
            cJSON_AddStringToObject(obj, "parent", Folder_get_absolute_path(file->parent));
        else
            cJSON_AddNullToObject(obj, "parent");

        cJSON_AddItemToArray(files_json, obj);
    }

    char *json = cJSON_Print(root);

    FILE *fp = fopen(get_save_path(), "w");
    if (fp) {
        fputs(json, fp);
        fclose(fp);
    }

    free(json);
    cJSON_Delete(root);
}

void load_fs(void) {
    FILE *fp = fopen(get_save_path(), "r");
    if (!fp) return;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *text = malloc(size + 1);
    fread(text, 1, size, fp);
    text[size] = '\0';
    fclose(fp);

    cJSON *root = cJSON_Parse(text);
    free(text);

    if (!root)
        return;

    /* ---------- Folders ---------- */

    cJSON *folders_json = cJSON_GetObjectItem(root, "folders");

    cJSON *folder;
    cJSON_ArrayForEach(folder, folders_json) {
        Folder *f = malloc(sizeof(Folder));

        f->name = strdup(cJSON_GetObjectItem(folder, "name")->valuestring);
        f->parent = NULL;

        Folder_add(f);
    }

    int i = 0;
    cJSON_ArrayForEach(folder, folders_json) {
        cJSON *parent = cJSON_GetObjectItem(folder, "parent");

        if (!cJSON_IsNull(parent))
            folders[i]->parent = get_folder_by_path(parent->valuestring);

        i++;
    }

    /* ---------- Files ---------- */

    cJSON *files_json = cJSON_GetObjectItem(root, "files");

    cJSON *file;
    cJSON_ArrayForEach(file, files_json) {
        File *f = malloc(sizeof(File));

        f->name = strdup(cJSON_GetObjectItem(file, "name")->valuestring);
        f->ext = strdup(cJSON_GetObjectItem(file, "ext")->valuestring);
        f->contents = strdup(cJSON_GetObjectItem(file, "contents")->valuestring);

        cJSON *parent = cJSON_GetObjectItem(file, "parent");
        if (!cJSON_IsNull(parent))
            f->parent = get_folder_by_path(parent->valuestring);
        else
            f->parent = NULL;

        File_add(f);
    }

    cJSON_Delete(root);
}
