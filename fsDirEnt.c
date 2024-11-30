/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fsDirEnt.c
*
* Description:: Implementing of the directory functions using
* prototypes found in the .h file
*
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsDirEnt.h"	// ensure this is always present!
#include "fsFAT.h"      // used for blockPOS!
#include "fsVCB.h"

#define BLOCKSIZE 512

// TODO implement createDirectory and any other helpers if needed
// then when we initialize the root we pass along to any of these and
// mount it in initFileSystem so its saved in the FAT and we
// can set the freeSpace info in the vcb!
dir_Entry* createDirectory(int NumofEntries, dir_Entry* parent)  // refer to 10/15 and 10/17 lecture to implement
{
    // Calculating how much memory is needed
    int bytesNeeded = NumofEntries * sizeof(dir_Entry);
    int numOfBlocks = ((bytesNeeded + (BLOCKSIZE - 1))/BLOCKSIZE); 
    int totalNumOfBytes = numOfBlocks * BLOCKSIZE;
    int actualNumOfEntries = totalNumOfBytes / sizeof(dir_Entry);

    // Allocating memory for new directory entries
    dir_Entry* new = malloc(totalNumOfBytes);
    if(new == NULL)
    {
        perror("FAILED TO ALLOCATE DIRECTORY");
        free(new);
        exit(-1);
    }
    int location = allocateBlocks(numOfBlocks, BLOCKSIZE);

    if(location <= 1)
    {
        location = 2;
    }
    printf("the block of the root is at %d", location);

    // Used to assign the current time to directores being created now
    time_t now = time(NULL);

/*
    // set all to free
    // Initliazing vales in the rest of the entries's
    for(int i = 2; i < actualNumOfEntries; i++)
    {
        new[i].creation_date = now;
        new[i].last_edited = now;
        new[i].last_accessed = now;
        new[i].is_Directory = 0;
        strcpy(new[i].name, "NO USED");
        new[i].size = 0;
        //TODO: Block position should be given by the allocateBlocks function
        new[i].blockPos = location;
    }
    */
    new[2].creation_date = now;
        new[2].last_edited = now;
        new[2].last_accessed = now;
        new[2].is_Directory = 1;
        strcpy(new[2].name, "dir");
        new[2].size = 0;
        //TODO: Block position should be given by the allocateBlocks function
        new[2].blockPos = location;

        new[3].creation_date = now;
        new[3].last_edited = now;
        new[3].last_accessed = now;
        new[3].is_Directory = 0;
        strcpy(new[3].name, "file");
        new[3].size = 0;
        //TODO: Block position should be given by the allocateBlocks function
        new[3].blockPos = location;

    // Intializing current dir (.)
    new[0].creation_date = now;
    new[0].last_edited = now;
    new[0].last_accessed = now;
    new[0].is_Directory = 1;
    strcpy(new[0].name, ".");
    new[0].blockPos = location; // will be changed once freespace management is fully done
    new[0].size = actualNumOfEntries * sizeof(dir_Entry);

    // Intializing parent dir (..)
    if(parent == NULL)
    {
        parent = new;
    }

    strcpy(new[1].name, "..");
    new[1].blockPos = parent[0].blockPos;
    new[1].size = parent[0].size;
    new[1].creation_date = parent[0].creation_date;
    new[1].last_edited = parent[0].last_edited;
    new[1].last_accessed = parent[0].last_accessed;
    new[1].is_Directory = parent[0].is_Directory;

    // Call LBA write for each dir entry created for discontigious allocation?
    LBAwrite(new, new[0].size + ((BLOCKSIZE-1)/BLOCKSIZE), new[0].blockPos);

    return new;
}

// 1 if it is a directory and 0 if not
int isDir(dir_Entry * entry){
if(entry->is_Directory == 1)
{
    return 1;
}
else
{
    return 0;
}
}

// Allows user to rename the Directory Entry
dir_Entry * renameDirEntry(int blockLocation)
{

}

// Allows user to assign the file type of a Directory Entry
int assignFile(int f)
{
return 0;
}
