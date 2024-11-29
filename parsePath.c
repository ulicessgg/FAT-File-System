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

// returns the index of where the dir is in the array
int parsePath(char * path, dir_Entry ** returnParent, int * index, char ** lastElement)
{
    // checking to see is paramters passed in are valid
    if(path == NULL)
    {
        return -1;
    }
    if(strlen(path) == 0)
    {
        return -1;
    }
    // Creates the start directory of where 
    // we will be searching through
    dir_Entry * start;

    // checking for absolute path
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
            printf("\nwe lit\n");
            * returnParent = parent;
            * index = dirIndex;
            * lastElement = token1;
            return 0;
        }

        // if it is not a directory then there is alredy a
        // file or invalid path
        else if(isDir(parent[dirIndex]) == 0)
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
