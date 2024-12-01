/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit 
* Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: parsePath.c
*
* Description:: implements the function that parses the 
* directory path given by the user. 
* 
*
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsFAT.h"	// ensure this is always present!
#include "fsVCB.h"
#include "fsDirEnt.h"




// iterates through the directory and tell us the index of the directory if the name is found
// if the name is not found in the parent dir then return -1
int findInDir(dir_Entry * parent, char * name)
{
    if(parent == NULL){
        return -1;
    }
    if(name == NULL){
        return -1;
    }

    int numOfEntries = (parent[0].size / sizeof(dir_Entry));

    for(int x=0; x < numOfEntries; x++)
    {
        // could create a function that checks if a dir is used or not and use that to
        // bring down the number of directores checked
        if(strcmp(parent[x].name, name) == 0)
        {
      
            return x;
        }
    }
    return -1;
}

// TODO:
// test this function once allocate blocks gives the correct location for the blocks
// of each dir_Entry
dir_Entry * loadDir(dir_Entry * entry)
{

    dir_Entry * newDir = malloc(sizeof(dir_Entry));
    if(newDir == NULL)
    {
        perror("FAILED TO ALLOCATE DIRECTORY");
        free(newDir);
        exit(-1);
    }
    
    LBAread(newDir,newDir->size, newDir->blockPos);

    return newDir;
}

// returns the index of where the dir is in the array
int parsePath(char * path, dir_Entry ** returnParent, int * index, char ** lastElement)
{
    // checking to see is paramters passed in are valid
    if(path == NULL || strlen(path) == 0)
    {
        return -1;
    }
    
    // Creates the start directory of where 
    // we will be searching through
    dir_Entry * start;

    // checking for absolute vs relaive path
    if(path[0] == '/')
    {
        
        // global variables will keep in main program
        start = root;
        printf("\nROOT NOW\n");
    }
    // checking for relative path
    else
    {
     printf("\ncwd");
        // global variables will keep in main program
        start = cwd;
    }

    if (start == NULL) { printf("Error: start directory is NULL\n"); return -1; }

    char* token1;
    char* token2;
    char* savePtr;
    dir_Entry * parent;
    parent = start;

    printf("\nParent name: %s", parent->name);

    token1 = strtok_r(path,"/", &savePtr);
    printf("\ntoken1: %s", token1);
    // Special case: the path given is only a "/"
    // meaning a directory
    if(token1 == NULL)
    {
        printf("\nSpecial case reached\n");
         * returnParent = parent;
         * index = 0;
         * lastElement = NULL;
         return 0;
    }
    token2 = strtok_r(path, "/", &savePtr);
    printf("\ntoken2: %s", token2);

    //loop while 
    while(token2 != NULL)
    {
            printf("\n enter loop\n");
        token2 = strtok_r(NULL, "/", &savePtr);
        int dirIndex = findInDir(parent, token1);

         // invalid path 
        if(dirIndex == -1)
        {
            return -1;
        }
       

        // gives the parent Dir, index of the element in the parent dir
        // and the name of that element
        // token 1 is the last element and we can use this info to do 
        // what the funciton needs to do
        if(token2 == NULL)
        {
            printf("\nworking\n");
            * returnParent = parent;
            * index = dirIndex;
            * lastElement = token1;
            printf("\n %s, %d, %s", *returnParent != NULL  ? (*returnParent)[0].name : "NULL parent", index != NULL ? *index : -12121, lastElement != NULL ? *lastElement : "NULL char");
            printf("\nstill working2\n");
            // this is the bad part
            return 0;
        }

        char* dirName = parent[dirIndex].name;

        // if it is not a directory then there is alredy a
        // file or invalid path
        if (fs_isDir(dirName) == 0)
        {
            return -1;
        }
        

        // with the valid path we load this new directory into memory
        dir_Entry * newParent = loadDir(&parent[dirIndex]);
        //freeDIr(parent)
        // adjusting new staring points
        parent = newParent;
        token1 = token2;
     
    }
    printf("\nend of loop\n");
}

// Helper to split a path into components // prash
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

// Locate a directory given its path // prash
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
