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

// Key directory functions
int fs_mkdir(const char *pathname, mode_t mode) // Marco
{
    // Need to make sure createDIr works after allocate blocks works
}

int fs_rmdir(const char *pathname) // yash
{
    if (!pathname) {
        fprintf(stderr, "Error: Directory path is NULL.\n");
        return -1;
    }

    dir_Entry *parentDir = NULL;
    int *dirIndex = NULL;
    char *lastElement = NULL;

    // Step 1: parse directory path to locate the directory and its parent
    if (parsePath(pathname, &parentDir, &dirIndex, &lastElement) != 0) {
        fprintf(stderr, "Error: Failed to parse directory path.\n");
        return -1;
    }

    // Step 2: verify that the path points to a directory
    if (!fs_isDir(pathname)) {
        fprintf(stderr, "Error: %s is not a directory.\n", pathname);
        return -1;
    }

    // Step 3: make sure that the directory is empty
    if (!is_directory_empty(pathname)) {
        fprintf(stderr, "Error: Directory %s is not empty.\n", pathname);
        return -1;
    }

    // Step 4: remove directory entry from parent directory
    if (remove_directory_entry(parentDir, *dirIndex) != 0) {
        fprintf(stderr, "Error: Failed to remove directory entry for %s.\n", pathname);
        return -1;
    }

    return 0;
}

// Directory iteration functions
fdDir* fs_opendir(const char *pathname) // prash
{
    if ((char *)pathname == NULL || strlen((char *)pathname) == 0)
    {
        fprintf(stderr, "Error: Invalid pathname\n");
        return NULL;
    }

    // Check if the pathname is a directory
    if (!fs_isDir((char *)pathname))
    {
        fprintf(stderr, "Error: Path is not a directory\n");
        return NULL;
    }

    int entryCount = 0;
    
    // i commented this out since it brings up an error when calling the helper
    // unsure if it is provided to us or if it has to be made but as of now it 
    // throws an error when testing the shell - ulices
    struct fs_diriteminfo *entries /* = fs_getDirEntries((char *)pathname, &entryCount) */;
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

struct fs_diriteminfo *fs_readdir(fdDir *dirp) // prash or yash
{
    /*  returns the wrong type and not using the prototype above ^

    // int readdir(const char *dirPath) do not use this edit pls

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
    */
}

int fs_closedir(fdDir *dirp) // prash
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
char* fs_getcwd(char *pathname, size_t size) // Marco
{
// temp fix
return "c";

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

int fs_isFile(char * filename)	//return 1 if file, 0 otherwise // marco
{
    if(filename == NULL || (strlen(filename) == 0))
    {
        return -1;
    }

    dir_Entry * entry = NULL;
    int* index = NULL;
    char * lastElement = "not here";
    printf("\n\n----here");
    int returnVal =121212;
     returnVal = parsePath(filename, &entry, &index, &lastElement);
    printf("----here2");
    if(returnVal == -1)
    {
         printf("hi");
        return -1;
    }

     printf("----here3");
    int returnIndex = *index;

    return entry[returnIndex].is_Directory;
}

// return 1 if directory, 0 otherwise , -1 failed to find directory
int fs_isDir(char * pathname)	// Marco	
{
    if(pathname == NULL || (strlen(pathname) == 0))
    {
        return -1;
    }

    dir_Entry * entry = NULL;
    int* index = NULL;
    char * lastElement = "not here";
    printf("\n\n----here");
    int returnVal =121212;
     returnVal = parsePath(pathname, &entry, &index, &lastElement);
    printf("----here2");
    if(returnVal == -1)
    {
         printf("hi");
        return -1;
    }

     printf("----here3");
    int returnIndex = *index;

    return entry[returnIndex].is_Directory;
}

int fs_delete(char* filename)	//removes a file // yash
{
    if (!filename) {
        fprintf(stderr, "Error: File path is NULL.\n");
        return -1;
    }

    dir_Entry *parentDir = NULL;
    int *dirIndex = NULL;
    char *lastElement = NULL;

    // Step 1: parse file path to locate file and parent directory
    if (parsePath(filename, &parentDir, &dirIndex, &lastElement) != 0) {
        fprintf(stderr, "Error: Failed to parse file path.\n");
        return -1;
    }

    // Step 2: verify that path points to a file
    if (!fs_isFile(filename)) {
        fprintf(stderr, "Error: %s is not a file.\n", filename);
        return -1;
    }

    // Step 3: remove file's entry from parent directory
    if (remove_directory_entry(parentDir, *dirIndex) != 0) {
        fprintf(stderr, "Error: Failed to remove directory entry for %s.\n", filename);
        return -1;
    }

    // Step 4: free disk space allocated to the file
    if (free_disk_blocks(filename) != 0) {
        fprintf(stderr, "Error: Failed to free disk blocks for %s.\n", filename);
        return -1;
    }

    return 0;
}

int fs_stat(const char *path, struct fs_stat *buf) // prash
{
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
    // buf->type = entry->is_Directory ? 1 : 0; // not using fs_stat struct 
    // buf->size = entry->size; // not using fs_stat struct either

    return 0; // Success
}


