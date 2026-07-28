

typedef struct Folder Folder;
typedef struct Folder {
    char* name;
    Folder* parent;
} Folder;

const char* Folder_get_absolute_path(Folder* folder);

typedef struct {
    char* name;
    char* ext;
    char* contents;
    Folder* parent;
} File;

const char* File_get_absolute_path(File* file);
