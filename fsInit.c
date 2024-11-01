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
#include <stdint.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsVCB.h"
#include "fsDirEnt.h"

/* USE THIS TO TEST THE HEXDUMP

Hexdump/hexdump.linux SampleVolume --start 1 --count 3

*/

// first 18 digits of pi as our signature
uint64_t signature = 314159265358979323;

// Global instances for system-wide use
volumeControlBlock* vcb;
int* FAT;
dir_Entry* root;

// step 1 in milestone 1
int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);

	// allocate the memory for the vcb
	vcb = malloc(blockSize);
	if(vcb == NULL)
	{
		perror("FAILED TO ALLOCATE THE VCB");
		free(vcb);
		exit(-1);
	}

	// read a block into the vcb
	LBAread(vcb, 1, 0);

	// check if the vcb has been initialized if it has proceed to end of init
	if(vcb->signature == signature)
	{
		printf("Volume Control Block Present!\n");
		LBAread(FAT, vcb->freeBlockCount, vcb->freeBlockStart);
		if(FAT == NULL)
		{
			perror("FAILED TO LOAD FAT");
			free(FAT);
			exit(-1);
		}

		LBAread(root, 1, vcb->rootLoc);
		if(root == NULL)
		{
			perror("FAILED TO LOAD ROOT");
			free(root);
			exit(-1);
		}
	}

	// if the vcb has not been initialized proceed to initializing its values
	// step 2 in milestone 1
	else
	{
		printf("Volume Control Block not Present!\n");
		
		// step 3 of milestone 1
		//Initialize the FAT as a linked list
        initFAT(numberOfBlocks);

		//allocate the memory for the vcb
		root = malloc(blockSize);
		if(root == NULL)
		{
			perror("FAILED TO ALLOCATE THE ROOT");
			free(root);
			exit(-1);
		}

		// step 4 of milestone 1
		// Create the root directory
		root = createDirectory(6, NULL); // 0 for now as we are not handling files yet

		// initialize the values in the volume control block
		vcb->totalBlocks = numberOfBlocks;
		vcb->blockSize = blockSize;
		vcb->freeBlockCount = numberOfBlocks - 1; 
		vcb->fatSize = numberOfBlocks - 1; 
		vcb->rootLoc = root->blockPos; 
		vcb->signature = signature;
		strcpy(vcb->sysType,"File Allocation Table");

		// write the vcb into block 0
		LBAwrite(vcb, 1, 0);
		LBAwrite(FAT, 1, 1);
		printf("Finished initializing VCB and FAT!\n");
	}

	return 0;
}

void exitFileSystem() 
{
    printf("System exiting\n");

    if (vcb != NULL) {
        free(vcb);
        vcb = NULL;
    }
    
    if (FAT != NULL) {
        free(FAT);
        FAT = NULL;
    }
    
    if (root != NULL) {
        free(root);
        root = NULL;
    }
}
