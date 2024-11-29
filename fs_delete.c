#include "fs_delete.h"
#include "parsePath.h"
#include "fsFAT.h"
#include "fsDirEnt.h"
#include <stdio.h>

int fs_delete(const char *file_path) {
    if (!file_path) {
        fprintf(stderr, "Error: File path is NULL.\n");
        return -1;
    }

    dir_Entry *parentDir = NULL;
    int *dirIndex = NULL;
    char *lastElement = NULL;

    // Parse the path to locate the file and its parent directory
    if (parsePath(file_path, &parentDir, &dirIndex, &lastElement) != 0) {
        fprintf(stderr, "Error: Failed to parse file path.\n");
        return -1;
    }

    // Ensure it's a file
    if (!fs_isFile(file_path)) {
        fprintf(stderr, "Error: %s is not a file.\n", file_path);
        return -1;
    }

    // Remove directory entry
    if (remove_directory_entry(parentDir, *dirIndex) != 0) {
        fprintf(stderr, "Error: Failed to remove directory entry for %s.\n", file_path);
        return -1;
    }

    // Free disk blocks associated with the file
    if (free_disk_blocks(file_path) != 0) {
        fprintf(stderr, "Error: Failed to free disk blocks for %s.\n", file_path);
        return -1;
    }

    return 0;
}
