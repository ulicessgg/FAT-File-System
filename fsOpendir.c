/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fsOpendir.c
*
* Description:: Implements the fs_opendir function 1 
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fsOpendir.h"    // Include the specific header file for this function

fdDir* fs_opendir(const char *pathname)
{
    if (pathname == NULL || strlen(pathname) == 0)
    {
        fprintf(stderr, "Error: Invalid pathname\n");
        return NULL;
    }

    // Check if the pathname is a directory
    if (!fs_isDir(pathname))
    {
        fprintf(stderr, "Error: Path is not a directory\n");
        return NULL;
    }

    int entryCount = 0;
    struct fs_diriteminfo *entries = fs_getDirEntries(pathname, &entryCount);
    if (entries == NULL || entryCount == 0)
    {
        fprintf(stderr, "Error: Failed to read directory entries\n");
        return NULL;
    }

    fdDir *dirp = (fdDir *)malloc(sizeof(fdDir));
    if (dirp == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    dirp->currentEntry = 0;
    dirp->totalEntries = entryCount;
    dirp->entries = entries;

    return dirp;
}
