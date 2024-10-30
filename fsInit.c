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
#include "fsVCB.h"		// ensure this is always present!
#include "fsDirEnt.h"	// ensure this is always present!

// to make sure that our vcb can be identified i decided to use the
// first 9 digits of pi as our signature
uint64_t signature = 314159265358979323;

// create a global instance of the vcb for system wide use
volumeControlBlock* vcb;

// since we are using a FAT we can do this using an int array
// leave as a pointer so we can resize it dynamically as needed
int* FAT;

// Define a node in the FAT linked list
typedef struct FATNode {
    uint64_t blockIndex;
    struct FATNode* next;
} FATNode;

// Global head of the linked list for free space tracking
FATNode* freeListHead = NULL; // Initialize as NULL at the start

// TODO inside of the fsDirEnt.c file implement createDirectory
// then when we initialize the root we can pass this along and
// mount it in initFileSystem so its saved in the FAT and we
// can set the freeSpace info in the vcb!
dir_Entry* root;

// Helper function to initialize the FAT as a linked list
void initFAT(uint64_t numberOfBlocks) {
    FATNode* prevNode = NULL;

    // Create a node for each block, starting from block 1 (block 0 is reserved for VCB)
    for (uint64_t i = 1; i < numberOfBlocks; i++) {
        FATNode* newNode = (FATNode*)malloc(sizeof(FATNode));
        if (newNode == NULL) {
            perror("Failed to allocate memory for FAT node");
            exit(-1);
        }
        
        // Initialize node properties
        newNode->blockIndex = i;
        newNode->next = NULL;
        
        // Link nodes together
        if (prevNode == NULL) {
            freeListHead = newNode;  // First node in the list becomes the head
        } else {
            prevNode->next = newNode;
        }
        
        prevNode = newNode;
    }
    
    printf("FAT linked list initialized with %lu blocks.\n", numberOfBlocks - 1);
}

// step 1 in milestone 1
int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */

	//allocate the memory for the vcb
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
	}
	// if the vcb has not been initialized proceed to initializing its values
	// step 2 in milestone 1
	else
	{
		printf("Volume Control Block not Present!\n");
		// initialize the values in the volume control block
		vcb->totalBlocks = numberOfBlocks;
		vcb->blockSize = blockSize;
		vcb->freeBlockCount = numberOfBlocks - 1; // TODO once the FAT has be initialized correct this
		vcb->freeBlockStart = 1; // TODO once the FAT has be initialized correct this
		// vcb->fatSize = vcb->freeBlockCount; // TODO once the FAT has be initialized correct this
		// vcb->rootLoc = root->blockPos; // TODO once the FAT has be initialized correct this
		vcb->signature = signature;
		strcpy(vcb->sysType,"File Allocation Table");

		// TODO initialize the free space (FAT) step 3 in milestone 1

		// TODO initialize the root directory step 4 in milestone 1

		printf("Finished initializing vcb!\n");

	}

	return 0;
}

//Initialize the file system, including VCB and FAT
int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize) {
    printf("Initializing File System with %lu blocks with a block size of %lu\n", numberOfBlocks, blockSize);

    //Allocate memory for the VCB
    vcb = (volumeControlBlock*)malloc(blockSize);
    if (vcb == NULL) {
        perror("FAILED TO ALLOCATE THE VCB");
        free(vcb);
        exit(-1);
    }

    //Read a block into the VCB
    LBAread(vcb, 1, 0);

    //Check if VCB is initialized
    if (vcb->signature == signature) {
        printf("Volume Control Block Present!\n");
    } else {
        printf("Volume Control Block not Present!\n");

        //Initialize VCB fields
        vcb->totalBlocks = numberOfBlocks;
        vcb->blockSize = blockSize;
        vcb->signature = signature;
        strcpy(vcb->sysType, "File Allocation Table");

        //Initialize the FAT as a linked list
        initFAT(numberOfBlocks);

        //Set VCB free block start to the index of the first free block
        vcb->freeBlockStart = freeListHead->blockIndex;

        //Set the size of FAT in blocks (each node represents one block)
        vcb->fatSize = numberOfBlocks - 1;
        vcb->freeBlockCount = numberOfBlocks - 1; // Exclude VCB block

        //TODO: Initialize the root directory (step 4 in milestone 1)

        printf("Finished initializing VCB and FAT as linked list!\n");
    }

    return 0;
}
	
void exitFileSystem ()
{
	printf ("System exiting\n");
	// write the vcb into block 0
	LBAwrite(vcb, 1, 0);
	free(vcb);
}