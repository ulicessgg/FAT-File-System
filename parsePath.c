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


int parsePath(char * path, dir_Entry * returnParent, int index, char ** lastElement)
{

// checking to see is paramters passed in are valid
if(path == null)
{
    return -1;
}
dir_Entry * start;
if(strlen(path) == 0)
{
    return -1;
}
// checking for absolute path
if(path[0] == '/')
{
    // global variables will keep in main program
    start = rootdir
}
// checking for relative path
else
{
    // global variables will keep in main program
    start == cwd
}

char* token1;
char* token2;
char* savePtr;
dir_Entry * parent;
parent = start;

token1 = strtok_r(path,"/", savePtr);
// the path is only a / meaning a directory
if(token1 == null)
{

}

token2 = strtok_r(null, "/", savePtr)
int dirIndex = findInDir(parent, token1)

// gives the parent Dir, index of the element in the parent dir
// and the name of that element
// token 1 is the last element and we can use this info to do 
// what the funciton needs to do
if(token2 == null)
{
    * returnParent = parent;
    * index = dirIndex;
    * lastElement = token1;
    return 0;
}

// invalid path 
if(dirIndex == -1)
{
    return -1;
}
// if it is not a directory then there is alredy a
// file or invalid path
else if(!(parent[dirIndex].isDir))
{
return -1;
}
{

}

// with the valid path we load this new directory into memory
dir_Entry * new parent = loadDir(&parent[dirIndex]);


}


// iterates through the directory 
int findInDir(dir_Entry * parent, char * token)
{

}