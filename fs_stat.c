/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fs_stat.c
*
* Description:: Implements the fs_stat function
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fs_stat.h"
#include "fsDirEnt.h" // For dir_Entry definition
#include "locateDirectory.h"

int fs_stat(const char *path, struct fs_stat *buf) {
    if (path == NULL || buf == NULL) {
        fprintf(stderr, "Error: Invalid arguments to fs_stat\n");
        return -1;
    }

    // Locate the file or directory
    dir_Entry *entry = locateDirectory(path); // Use locateDirectory to resolve the path
    if (entry == NULL) {
        fprintf(stderr, "Error: Path not found\n");
        return -1;
    }

    // Populate the fs_stat structure
    buf->type = entry->is_Directory ? FS_TYPE_DIR : FS_TYPE_FILE;
    buf->size = entry->size;

    return 0; // Success
}
