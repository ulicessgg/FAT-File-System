//fsFAT.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fsFAT.h"
#include "fsVCB.h"
#include "fsDirEnt.h"

//Global declarations
volumeControlBlock* vcb;
const uint64_t signature = 314159265358979323;

FATNode* freeListHead = NULL; //Initialize the free list head



void freeFAT() {
    FATNode* current = freeListHead;
    while (current != NULL) {
        FATNode* temp = current;
        current = current->next;
        free(temp);
    }
    freeListHead = NULL; // Reset head pointer
}

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