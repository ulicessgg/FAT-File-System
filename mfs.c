/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: mfs.h
*
* Description:: 
*	This is the file system interface.
*	This is the interface needed by the driver to interact with
*	your filesystem.
*
**************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "b_io.h"
#include "fsDirEnt.h"
#include "mfs.h"
#include "parsePath.h"
#include "fsLow.h" // Assuming this contains low-level filesystem utilities

// Global variable to track the current working directory
static char currentWorkingDir[MAX_PATH_LENGTH] = "/";
dir_Entry * root;
dir_Entry * cwd;

// Key directory functions
int fs_mkdir(const char *pathname, mode_t mode) // marco
{
    
}

int fs_rmdir(const char *pathname); // optional

// Directory iteration functions
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

struct fs_diriteminfo *fs_readdir(fdDir *dirp) // prash
{

}

int fs_closedir(fdDir *dirp)
{
    if (dirp == NULL)
    {
        fprintf(stderr, "Error: Invalid directory pointer\n");
        return -1;
    }

    free(dirp->entries);
    free(dirp);

    return 0;
}

// Misc directory functions
char* fs_getcwd(char *pathname, size_t size) // marco
{

}

int fs_setcwd(char *pathname)   //linux chdir // prash
{
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

int fs_isFile(char * filename)	//return 1 if file, 0 otherwise // prash
{

}

int fs_isDir(char * pathname)		//return 1 if directory, 0 otherwise // marco
{

    if(pathname[0] == NULL || (strlen(pathname) == 0))
    {
        return -1;
    }

    dir_Entry * entry = NULL;
    int * index = -1;
    char * lastElement = "not here";

    int returnVal = parsePath(pathname, &entry, &index, &lastElement);
    //debugging
    printf("Path Name: %s\nReturn val: %d\nIndex: %d\nLast Elem: %s",pathname,returnVal, index, lastElement);

return returnVal;

}

int fs_delete(char* filename);	//removes a file // optional

int fs_stat(const char *path, struct fs_stat *buf) // yash
{

}


