/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fsClosedir.c
*
* Description:: Implements the fs_closedir function 1
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "fsClosedir.h"    // Include the specific header file for this function

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
