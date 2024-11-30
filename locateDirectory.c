/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: locateDirectory.c
*
* Description:: Implements the locateDirectory helper function
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fsDirEnt.h"
#include "fsFAT.h"
#include "fsVCB.h"
#include "mfs.h"

// Helper to split a path into components
static char** splitPath(const char *path, int *numComponents) {
    if (path == NULL || strlen(path) == 0) {
        *numComponents = 0;
        return NULL;
    }

    char *pathCopy = strdup(path); // Duplicate path to modify it
    if (pathCopy == NULL) {
        return NULL;
    }

    int capacity = 10; // Initial capacity for components array
    char **components = (char **)malloc(capacity * sizeof(char *));
    if (components == NULL) {
        free(pathCopy);
        return NULL;
    }

    int count = 0;
    char *token = strtok(pathCopy, "/");
    while (token != NULL) {
        if (count >= capacity) {
            capacity *= 2;
            components = (char **)realloc(components, capacity * sizeof(char *));
            if (components == NULL) {
                free(pathCopy);
                return NULL;
            }
        }
        components[count++] = strdup(token);
        token = strtok(NULL, "/");
    }

    *numComponents = count;
    free(pathCopy);
    return components;
}

// Locate a directory given its path
dir_Entry* locateDirectory(const char *dirPath) {
    if (dirPath == NULL || strlen(dirPath) == 0) {
        fprintf(stderr, "Error: Invalid directory path\n");
        return NULL;
    }

    int numComponents = 0;
    char **components = splitPath(dirPath, &numComponents);
    if (components == NULL || numComponents == 0) {
        fprintf(stderr, "Error: Failed to parse directory path\n");
        return NULL;
    }

    // Start at the root directory
    dir_Entry *currentDir = getRootDirectory(); // Assuming this function retrieves the root directory
    if (currentDir == NULL) {
        fprintf(stderr, "Error: Root directory not found\n");
        return NULL;
    }

    for (int i = 0; i < numComponents; i++) {
        dir_Entry *nextDir = findDirectoryEntry(currentDir, components[i]); // Locate subdirectory
        if (nextDir == NULL || nextDir->is_Directory == 0) {
            fprintf(stderr, "Error: Component '%s' not found or is not a directory\n", components[i]);
            currentDir = NULL;
            break;
        }
        currentDir = nextDir; // Move to the next directory
    }

    // Clean up components
    for (int i = 0; i < numComponents; i++) {
        free(components[i]);
    }
    free(components);

    return currentDir;
}
