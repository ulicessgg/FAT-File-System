/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit 
* Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fs_rmdir.c
*
* Description:: implementation for fs_rmdir, a directory
* operation that deletes an instance of a directory.
* locates a directory, ensures that it's empty, and
* removes entry from parent firectory.includes error
* handling for invalid paths, non-existent directories, and
* non-empty directories
**************************************************************/

#include "fs_rmdir.h"
#include "parsePath.h"
#include "fsDirEnt.h"
#include <stdio.h>

int fs_rmdir(const char *dir_path) {
    if (!dir_path) {
        fprintf(stderr, "Error: Directory path is NULL.\n");
        return -1;
    }

    dir_Entry *parentDir = NULL;
    int *dirIndex = NULL;
    char *lastElement = NULL;

    // Step 1: parse directory path to locate the directory and its parent
    if (parsePath(dir_path, &parentDir, &dirIndex, &lastElement) != 0) {
        fprintf(stderr, "Error: Failed to parse directory path.\n");
        return -1;
    }

    // Step 2: verify that the path points to a directory
    if (!fs_isDir(dir_path)) {
        fprintf(stderr, "Error: %s is not a directory.\n", dir_path);
        return -1;
    }

    // Step 3: make sure that the directory is empty
    if (!is_directory_empty(dir_path)) {
        fprintf(stderr, "Error: Directory %s is not empty.\n", dir_path);
        return -1;
    }

    // Step 4: remove directory entry from parent directory
    if (remove_directory_entry(parentDir, *dirIndex) != 0) {
        fprintf(stderr, "Error: Failed to remove directory entry for %s.\n", dir_path);
        return -1;
    }

    return 0;
}
