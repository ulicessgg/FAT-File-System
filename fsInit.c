/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fsInit.c
*
* Description:: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
*
**************************************************************/


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsVCB.h"		// ensure this is always present! - ulices
#include "fsDirEnt.h"	// ensure this is always present! - ulices

// to make sure that our vcb can be identified i decided to use the
// first 9 digits of pi as our signature
unsigned long signature = 314159265;
// create a global instance of the vcb for system wide use - ulices
volumeControlBlock* vcb;


int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */

	//allocate the memory for the vcb
	vcb = malloc(blockSize);
	if(vcb == NULL)
	{
		perror("FAILED TO ALLOCATE THE VCB");
		exit(-1);
	}

	// read a block into the vcb
	LBAread(vcb, 1, 0);

	// check if the vcb has been initialized if it has proceed to end of init
	if(vcb->signature == signature)
	{
		printf("Volume Control Block Present!\n");
	}
	// if the vcb has not been initialized proceed to initializing its values
	else
	{
		vcb->totalBlocks = numberOfBlocks;
		vcb->blockSize = blockSize;
		// freeBlockCount = numberOfBlocks - 1; // TODO once the FAT has be initialized correct this
		// vcb->freeBlockStart = 1; // TODO once the FAT has be initialized correct this
		// vcb->fatSize = freeBlockCount; // TODO once the FAT has be initialized correct this
		// vcb->rootLoc = 0; // TODO once the FAT has be initialized correct this
		vcb->signature = signature;
		strcpy(vcb->sysType,"File Allocation Table");
	}

	return 0;
}
	
	
void exitFileSystem ()
{
	printf ("System exiting\n");
}