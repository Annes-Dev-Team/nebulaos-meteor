#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#pragma once

void kernel_init();

typedef struct Folder Folder;
typedef struct Folder {
    char* name;
    Folder* parent;
} Folder;

const char* Folder_get_absolute_path(Folder* folder);
void Folder_add(Folder* folder);
Folder* get_folder_by_path(const char* path);

typedef struct {
    char* name;
    char* ext;
    unsigned char* contents;
    uint32_t size;
    Folder* parent;
} File;

const char* File_get_absolute_path(File* file);
void File_add(File* file);
File* get_file_by_path(const char* path);

typedef struct {
    char* username;
    char* displayname;
    bool isadmin;

} User;

extern Folder** folders;
extern File** files;

extern size_t folder_count;
extern size_t file_count;

void save_fs(void);
void load_fs(void);
