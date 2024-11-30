/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fsReadDir.c
*
* Description:: implements the readdir function
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fsReadDir.h"
#include "fsDirEnt.h"
#include "fsFAT.h"
#include "fsVCB.h"
#include "mfs.h"
#include "locateDirectory.h"


// The function used to read and print directory entries
int readdir(const char *dirPath) {
    if (dirPath == NULL) {
        fprintf(stderr, "Error: Directory path is NULL\n");
        return -1;
    }

    // Load the directory entry
    dir_Entry *currentDir = locateDirectory(dirPath);
    if (currentDir == NULL) {
        fprintf(stderr, "Error: Directory not found\n");
        return -1;
    }

    if (currentDir->is_Directory != 1) {
        fprintf(stderr, "Error: Path is not a directory\n");
        return -1;
    }

    // Load the entries in the directory
    unsigned int numEntries = currentDir->size / sizeof(dir_Entry);
    dir_Entry *entries = (dir_Entry *)malloc(currentDir->size);
    if (entries == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return -1;
    }

    if (readDirectoryEntries(currentDir->blockPos, entries, numEntries) < 0) {
        fprintf(stderr, "Error: Failed to read directory entries\n");
        free(entries);
        return -1;
    }

    // Print directory entries
    printf("Contents of directory '%s': \n", dirPath);
    for (unsigned int i = 0; i < numEntries; i++) {
        if (strlen(entries[i].name) > 0) { // Check if entry is valid
            printf("%s\t%s\t%u bytes\n",
            entries[i].is_Directory ? "[DIR]" : "[FILE]",
            entries[i].name,
            entries[i].size);
        }
    }

    free(entries);
    return 0;
}