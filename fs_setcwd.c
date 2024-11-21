/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fsSetcwd.c
*
* Description:: Implements the fs_setcwd function
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fs_setcwd.h"
#include "fsLow.h" // Assuming this contains low-level filesystem utilities

// Global variable to track the current working directory
static char currentWorkingDir[MAX_PATH_LENGTH] = "/";

int fs_setcwd(char *pathname) {
    if (pathname == NULL || strlen(pathname) == 0) {
        fprintf(stderr, "Error: Invalid pathname\n");
        return -1;
    }

    // Check if the given pathname is a directory
    if (!fs_isDir(pathname)) {
        fprintf(stderr, "Error: Path is not a directory\n");
        return -1;
    }

    // Update the current working directory
    if (strlen(pathname) >= MAX_PATH_LENGTH) {
        fprintf(stderr, "Error: Path exceeds maximum length\n");
        return -1;
    }

    strncpy(currentWorkingDir, pathname, MAX_PATH_LENGTH - 1);
    currentWorkingDir[MAX_PATH_LENGTH - 1] = '\0'; // Ensure null-termination

    return 0; // Success
}
