/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fsFAT.c
*
* Description:: implements the initialization of the FAT
* as well as the allocation of blocks for directory creation
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

void initFAT(uint64_t numberOfBlocks) {
    // Initialize FAT with -1 (indicating all blocks are free)
    for (uint64_t i = 0; i < numberOfBlocks; i++) {
        FAT[i] = 1;
    }
	// Set the first value in the FAT as -1 to mark the VCB location
	FAT[0] = -1;
    FAT[1] = -2;
    printf("FAT array initialized with %lu blocks.\n", numberOfBlocks);
}

// Function to find and allocate a single free block
int allocateBlock() {
    int block = 01;
    for (uint64_t i = 2; i < vcb->totalBlocks; i++) {
        if (FAT[i] == 1) { // Block is free
            FAT[i] = 2;    // Mark block as EOF if it's single block allocation
            vcb->freeBlockCount--;  // Update dree block count in VCB
            return i;
        }
    }
    return -1;
}

// Function to allocate multiple blocks for a file, returns starting block index
int allocateBlocks(uint64_t blockCount, uint64_t blockSize) {
	int startBlock = -1, prevBlock = -1;
	for (uint64_t i = 2; i < blockCount; i++) {
		int currentBlock = allocateBlock();
		if (currentBlock == -1) {
			fprintf(stderr, "Error: Not enough free blocks available\n");
			exit(-1); // not enough blocks are availble
		}

		if (i==0) {
			startBlock = currentBlock; // Start of file
		} else {
			FAT[prevBlock] = currentBlock; // Link the previous block to the current
		}
		prevBlock = currentBlock; // Update previous block
	}

	FAT[prevBlock] = -1; // Mark EOF chain
	return startBlock; // Return the starting block
}