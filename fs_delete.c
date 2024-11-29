/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit 
* Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fs_delete.c
*
* Description:: implements the fs_delete function, a file
* operation to delete a specific file. locates a file in 
* filesystem, removes its entry from parent directory, and
* frees up disk space associated with the file.
**************************************************************/

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

    // Step 1: parse file path to locate file and parent directory
    if (parsePath(file_path, &parentDir, &dirIndex, &lastElement) != 0) {
        fprintf(stderr, "Error: Failed to parse file path.\n");
        return -1;
    }

    // Step 2: verify that path points to a file
    if (!fs_isFile(file_path)) {
        fprintf(stderr, "Error: %s is not a file.\n", file_path);
        return -1;
    }

    // Step 3: remove file's entry from parent directory
    if (remove_directory_entry(parentDir, *dirIndex) != 0) {
        fprintf(stderr, "Error: Failed to remove directory entry for %s.\n", file_path);
        return -1;
    }

    // Step 4: free disk space allocated to the file
    if (free_disk_blocks(file_path) != 0) {
        fprintf(stderr, "Error: Failed to free disk blocks for %s.\n", file_path);
        return -1;
    }

    return 0;
}
