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

// add helper functions: for lbaread, lbawrite, extend, and trim
// root directory, how many blocks to write to disk, and buffer i will copy from

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsFAT.h"	// ensure this is always present!
#include "fsVCB.h"

int initFAT(uint64_t numBlocks, uint64_t lastBlock) {
    int requiredBytes = numBlocks * sizeof(int);
    int blocksOccupied = (requiredBytes + lastBlock - 1) / lastBlock;

    //debugging
    //printf("Calculating blocksNeeded: vcb->blockSize = %1d\n", vcb->blockSize);

    // Set the link values 
    for (int i = 0; i < numBlocks; i++) {
        if (i == 0 || i == blocksOccupied || i == numBlocks) {
            FAT[i] = 0xBE1355;
        }
        else {
            FAT[i] = i+1;
        }
    }

    // update the vcb with fat info
    vcb->freeBlockCount = numBlocks - blocksOccupied - 1;
    vcb->fatSize = blocksOccupied;
    vcb->fatLoc = 1;
    vcb->freeBlockStart = blocksOccupied + 1;

    // Write FAT to disk and check for errors
    if (LBAwrite(FAT, blocksOccupied, 1) == -1)
    {
        return -1;
    }

    return 0;
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

// Helper function for LBAread
int readBlocks(uint64_t rootBlock, uint64_t numBlocks, void* buffer) {
    if (numBlocks == 0 || buffer == NULL) {
        return -1; // Invalid input
    }
 
    uint64_t currentBlock = rootBlock;
    uint64_t blocksRead = 0;
    uint64_t bufferOffset = 0;

    while (blocksRead < numBlocks) {
        // Read a single block into the buffer at the correct offset
        uint64_t bytesRead = LBAread((char*)buffer + bufferOffset, 1, currentBlock);
        if (bytesRead != 1) {
            return -2; // Error during read
        }

        // Move to the next block in the FAT
        currentBlock = FAT[currentBlock];
        if (currentBlock == 0) {
            return -3; // End of chain reached prematurely
        }

        blocksRead++;
        bufferOffset += vcb->blockSize; 
    }

    return 0; // Success
}

// Helper function for LBAwrite
int writeBlocks(uint64_t rootBlock, uint64_t numBlocks, void* buffer) {
    if (numBlocks == 0 || buffer == NULL) {
        return -1; // Invalid input
    }

    uint64_t currentBlock = rootBlock;
    uint64_t blocksWritten = 0;
    uint64_t bufferOffset = 0;

    while (blocksWritten < numBlocks) {
        // Write a single block from the buffer at the correct offset
        uint64_t bytesWritten = LBAwrite((char*)buffer + bufferOffset, 1, currentBlock);
        if (bytesWritten != 1) {
            return -2; // Error during write
        }

        // Move to the next block in the FAT or allocate a new block
        if (FAT[currentBlock] == 0) { // End of chain
            FAT[currentBlock] = allocateBlock(); // Allocate a new block
            if (FAT[currentBlock] == -1) {
                return -3; // Allocation failed
            }
        }

        currentBlock = FAT[currentBlock];
        blocksWritten++;
        bufferOffset += vcb->blockSize; // Advance buffer offset by block size
    }

    return 0; // Success
}