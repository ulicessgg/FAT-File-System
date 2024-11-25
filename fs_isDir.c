/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fs_isDir.c
*
* Description:: Implements the fs_isDir function
**************************************************************/

#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsFAT.h"	// ensure this is always present!
#include "fsVCB.h"
#include "fsDirEnt.h"

int fs_isDir(char * pathname)
{

  // checking to see is paramters passed in are valid
    if(pathname == NULL)
    {
        return -1;
    }
    if(strlen(pathname) == 0)
    {
        return -1;
    }

dir_Entry * tempDir;
int index;
char * lastElem

int result = parsePath(pathname, tempDir, index, lastElem);


}
