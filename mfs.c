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
*	Implementation of file system interface needed by the driver
*   to interact with our filesystem.
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
    // Checking paramters passed in are valid
    if(pathname == NULL )
    {
        printf("Error: path name is null");
        return -1;
    }

    // Looking for the parent directory and l
    // location for destination of new 
    // directory entry
    dir_Entry * parent = NULL;
    int* index = NULL;
    char * lastElement = "not here";
    int returnVal =121212;
    returnVal = parsePath((char *)pathname, &parent, &index, &lastElement);

    // Cecking if a directory with the same name exists
    if(returnVal == 0 && parent->is_Directory == 1)
    {
        printf("\nDirectory already created with this name.\n");
        return -1;
    }

    // creating new directory
    dir_Entry * newDir = createDirectory(2,parent);
    if(newDir == NULL)
    {
        printf("\nError creating new Dir.\n");
        return -1;
    }

    // Adding new data of the created dir to the parent
    strcpy( (char*) pathname, parent[*index].name);
    parent[*index].is_Directory = 1;
    parent[*index].last_edited = newDir->creation_date;

    return 0;

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
    if (parsePath((char *) pathname, &parentDir, &dirIndex, &lastElement) != 0) {
        fprintf(stderr, "Error: Failed to parse directory path.\n");
        return -1;
    }

    // Step 2: verify that the path points to a directory
    if (!fs_isDir((char *) pathname)) {
        fprintf(stderr, "Error: %s is not a directory.\n", pathname);
        return -1;
    }

    /* Helper functions either havent been implemented or do not exist
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
    */

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

struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    if (dirp == NULL) {
        fprintf(stderr, "Error: Invalid directory pointer\n");
        return NULL;
    }

    // Check if there are more entries to read
    if (dirp->currentEntry >= dirp->totalEntries) {
        return NULL; // No more entries
    }

    // Retrieve the current entry
    struct fs_diriteminfo *currentEntry = &dirp->entries[dirp->currentEntry];

    // Move to the next entry for the next call
    dirp->currentEntry++;

    return currentEntry;
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
    // Copying the current workind directory to 
    // the buffer user passed in
strncpy(pathname, cwd.name, strlen(pathname));
return pathname;
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

//return 1 if file, 0 otherwise, -1 failed to find dir
int fs_isFile(char * filename)	 // marco
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
    if(entry[returnIndex].is_Directory == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
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
    /* Helper functions either havent been implemented or do not exist
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
    */
    return 0;
}

int fs_stat(const char *path, struct fs_stat *buf) {
    if (path == NULL || buf == NULL) {
        fprintf(stderr, "Error: Invalid arguments to fs_stat\n");
        return -1;
    }

    // Locate the directory or file entry
    dir_Entry *entry = locateDirectory(path); // Assume this function resolves the path
    if (entry == NULL) {
        fprintf(stderr, "Error: Path not found\n");
        return -1;
    }

    // Populate the fs_stat structure
    buf->st_size = entry->size;                // File or directory size in bytes
    buf->st_blksize = 512;                     // Assuming a fixed block size
    buf->st_blocks = (entry->size + 511) / 512; // Number of blocks (round up)
    buf->st_accesstime = entry->last_accessed; // Last accessed time
    buf->st_createtime = entry->creation_date; // Creation time
    return 0; // Success
}