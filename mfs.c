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
#include "fsLow.h" // Assuming this contains low-level filesystem utilities

// Global variable to track the current working directory
static char currentWorkingDir[MAX_PATH_LENGTH] = "/";

// Key directory functions
int fs_mkdir(const char *pathname, mode_t mode) // marco
{
    
}

int fs_rmdir(const char *pathname); // optional

// Directory iteration functions
dir_Entry* fs_opendir(const char *pathname) {
    // Validate pathname
    if (!pathname || strlen(pathname) == 0) {
        printf("Invalid pathname provided to fs_opendir.\n");
        return NULL;
    }

    // Locate directory entry
    dir_Entry *dir_entry = find_directory_entry(pathname);
    if (!dir_entry || !dir_entry->is_Directory) {
        printf("Directory not found or is not a directory.\n");
        return NULL;
    }

    // Return directory entry
    return dir_entry;
}
struct fs_diriteminfo *fs_readdir(fdDir *dirp) // prash
{

}

int fs_closedir(fdDir *dirp)
{
   if (!dirp) {
        printf("Invalid directory pointer passed to fs_closedir.\n");
        return -1; 
    }

    // Free memory allocated for `di`
    if (dirp->di) {
        free(dirp->di);
        dirp->di = NULL;
    }

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

}

int fs_delete(char* filename);	//removes a file // optional

int fs_stat(const char *path, struct fs_stat *buf) // yash
{

}


