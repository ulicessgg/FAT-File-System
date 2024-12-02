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

#define FAT_RESERVED 0xBE1355
#define FAT_FREE 0
#define FAT_EOF -1
#define BLOCKSIZE 512

int initFAT(uint64_t numBlocks, uint64_t blockSize) {
    int requiredBytes = numBlocks * sizeof(int);
    int blocksOccupied = (requiredBytes + blockSize - 1) / blockSize;
    int lastFreeBlock = numBlocks - 2;

    // Initialize FAT and free-space chain
    for (uint64_t i = 0; i < numBlocks; i++) {
        if (i < blocksOccupied) {
            FAT[i] = FAT_RESERVED; // Reserved for system use
        } else if (i == lastFreeBlock) {
            FAT[i] = FAT_EOF; // Mark the last free block as EOF
        } else if (i == numBlocks - 1) {
            FAT[i] = FAT_RESERVED; // Mark the final block as reserved
        } else {
            FAT[i] = i + 1; // Link free blocks in a chain
        }

        if (FAT[i] != FAT_RESERVED && FAT[i] != FAT_EOF && (FAT[i] < 0 || FAT[i] > numBlocks)){
            FAT[i] = FAT_FREE;
        }
    }

    // Update VCB
    vcb->freeBlockCount = numBlocks - blocksOccupied - 1;
    vcb->fatSize = blocksOccupied;
    printf("FAT SIZEDEBUGGG: %d\n\n", blocksOccupied);
    vcb->fatLoc = 1;
    vcb->freeBlockStart = blocksOccupied;

    // Write FAT to disk
    if (LBAwrite(FAT, vcb->fatSize, vcb->fatLoc) == -1) {
        fprintf(stderr, "Error: Failed to write FAT to disk.\n");
        return -1;
    }

    int written = LBAwrite(FAT, vcb->fatSize, vcb->fatLoc);
    printf("FAT written to disk: %d blocks\n", written);
    if (written != vcb->fatSize) {
        fprintf(stderr, "Error writing FAT to disk]\n");
    }

    // debugging
    printf("freeBlockCount: %u\n", vcb->freeBlockCount);
    printf("numBlocks: %lu\n", numBlocks);
    printf("blocksOccupied: %u\n", blocksOccupied);

    for (uint64_t i=0; i<numBlocks; i++) {
        if (FAT[i] != FAT_RESERVED && FAT[i] != FAT_EOF && FAT[i] != i + 1) {
            FAT[i] = FAT_FREE; // Mark unlinked blocks as free
        }
    }

    // debugging
    printf("freeBlockCount: %u\n", vcb->freeBlockCount);
    printf("numBlocks: %lu\n", numBlocks);
    printf("blocksOccupied: %d\n", blocksOccupied);
    printf("Last free block: %lu, FAT[%lu] = %d\n",
           (unsigned long)lastFreeBlock, (unsigned long)lastFreeBlock, FAT[lastFreeBlock]);


    return 0; // Success
}

void verifyFreeSpaceChain() { // used for debugging
    uint64_t current = vcb->freeBlockStart;
    uint64_t freeCount = 0;

    printf("Verifying Free-Space Chain:\n");
    while (current != FAT_EOF) {
        printf("Block %lu -> %u\n", current, FAT[current]);
        freeCount++;
        if (freeCount > vcb->freeBlockCount) {
            fprintf(stderr, "Error: Free-space chain exceeds expected count.\n");
            return;
        }
        current = FAT[current];
    }

    printf("Verified free-space chain ends correctly at block %lu.\n", current);
    if (freeCount != vcb->freeBlockCount) {
        fprintf(stderr, "Mismatch: Free block count (%lu) does not match VCB (%u).\n", freeCount, vcb->freeBlockCount);
    } else {
        printf("Free block count matches: %lu.\n", freeCount);
    }
}



void debugPrintFAT() { // used for debugging
    printf("FAT Contents:\n");
    for(uint64_t i=0; i<vcb->totalBlocks; i++) {
        printf("FAT[%lu]=%d\n", i, FAT[i]);
    }
    printf("Free Block Start: %u\n", vcb->freeBlockStart);
    printf("Free Block Count: %u\n", vcb->freeBlockCount);
}

void debugFreeSpaceChain() { // used for debugging
    uint64_t current = vcb->freeBlockStart;
    printf("Free-Space Chain:\n");
    while (current != FAT_EOF) {
        printf("Block %lu -> %u\n", current, FAT[current]);
        current = FAT[current];
    }
    printf("End of free-space chain at block %lu\n", current);
}

// Function to find and allocate a single free block
int allocateBlock(int * bufferOfFat, int currentIndex, int bufferSize) {
    for (int i = currentIndex ; i < bufferSize; i++) {
        if (bufferOfFat[i] == FAT_FREE) {
            return i;
        }
    }
    return -1;
}


// Function to allocate multiple blocks for a file, returns starting block index
// blockCount = number of blocks you want to take
// minContigious = the min ammount of blocks you want to be contigious
int allocateBlocks(uint64_t blockCount, uint64_t minContigiuos) {

    // Calculateing and allocating a buffer to hold the 
    // total size of the fat
    int bufferSize = vcb->fatSize*BLOCKSIZE;
    int * bufferOfFat = malloc(bufferSize);
    int congtigiousIndex = 0;


    //check to see if right calculation for # of int;s in the buff
    printf("\n# of Int's in buff: %d\n", (bufferSize)/4);

    // Finding first freeblock
    for(int i =0; i <= bufferOfFat[(bufferSize)/4]; i++)
    {
        // If the block is free we need to check if there is
        // congigious space for the rest of the disk
        if(bufferOfFat[congtigiousIndex] == 0)
        {
            // Now we know the index we are at refers to 
            // a free block, now have to check for a 
            // contigious list of free blocks
            int tempIndex = congtigiousIndex;
            int temp = 0;
            for(int x=0; x<= minContigiuos; x++)
            {
                if(bufferOfFat[tempIndex] != 0 ||  x == minContigiuos)
                {
                    break;
                }
                tempIndex++;
                temp++;
            }
            // Case that there is a list of contigious blocks
            // means that we can use the index found from the
            // first for loop
            if(temp == minContigiuos)
            {
                break;
            }

        }
        congtigiousIndex++;
    }


     // Finding first freeblock
    for(int i =0; i <= 12; i++)
    {
        // If the block is free we need to check if there is
        // congigious space for the rest of the disk
        if(bufferOfFat[congtigiousIndex] == 0)
        {
            // Now we know the index we are at refers to
            // a free block, now have to check for a
            // contigious list of free blocks
            int tempIndex = congtigiousIndex;
            int temp = 0;
            for(int x=0; x<= minContigiuos; x++)
            {
                if(bufferOfFat[tempIndex] != FAT_FREE ||  x == minContigiuos)
                {
                    break;
                }
                tempIndex++;
                temp++;
            }
            // Case that there is a list of contigious blocks
            // means that we can use the index found from the
            // first for loop
            if(temp == minContigiuos)
            {
                break;
            }

        }
        congtigiousIndex++;
    }

    int index = congtigiousIndex;
    

    while (blockCount > 0)
    {
        int nextBlockChain = -1;

        // looping through the number of blocks we need
        // leaving out the last block
        if (blockCount > 1)
        {
            nextBlockChain = allocateBlock(bufferOfFat, index+1, bufferSize);
            if (nextBlockChain == -1) {
                printf("\nNo more available blocks\n");
                break;
            }
        }
        // When we have reached the EOF
        else
        {
            nextBlockChain = FAT_EOF;
        }

        bufferOfFat[index] = nextBlockChain;
        index = nextBlockChain;
        blockCount--;
        if (nextBlockChain == FAT_EOF) {
            break;
        }
    }
	
	return 5;
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
        // CHECK TO SEE IF the PARAMTERS WORK:
            FAT[currentBlock] = allocateBlock(FAT, currentBlock, (sizeof(FAT)*sizeof(int))); // Allocate a new block
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