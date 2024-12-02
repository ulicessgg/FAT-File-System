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

/* USE THIS TO TEST THE HEXDUMP

Hexdump/hexdump.linux SampleVolume --start 1 --count 3
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsFAT.h"	// ensure this is always present!
#include "fsVCB.h"

#define FAT_FREE -1          // Mark free blocks
#define FAT_EOF -2  // End of a chain (EOF)
#define FAT_RESERVED -3      // Reserved for metadata

#define BLOCKSIZE 512

extern int *FAT; // Pointer to the FAT array
int initFAT(uint64_t numBlocks, uint64_t blockSize) {
    uint64_t requiredBytes = numBlocks * sizeof(int);
    uint64_t blocksOccupied = (requiredBytes + blockSize - 1) / blockSize;

    FAT = (int *)malloc(numBlocks * sizeof(int));
    if (FAT == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for FAT.\n");
        return -1;
    }

    // Initialize the reserved blocks
    for (uint64_t i = 0; i < blocksOccupied; i++) {
        FAT[i] = (i < blocksOccupied - 1) ? (i + 1) : FAT_RESERVED;
    }

    // Initialize the free blocks
    for (uint64_t i = blocksOccupied; i < numBlocks; i++) {
        FAT[i] = FAT_FREE;
    }

    // Make sure that the last block in free space is marked as EOF
    if (vcb->freeBlockCount > 0) {
        uint64_t lastFreeBlock = blocksOccupied + vcb->freeBlockCount - 1;
        FAT[lastFreeBlock] = FAT_EOF; // Mark last free block as EOF
        printf("Last free block (FAT[%lu]) marked as EOF: %d\n", lastFreeBlock, FAT[lastFreeBlock]);
    }

    //update VCB
    vcb->fatLoc = 0;
    vcb->fatSize = blocksOccupied;
    vcb->freeBlockStart = blocksOccupied;
    vcb->freeBlockCount = numBlocks - blocksOccupied;

    for (uint64_t i = blocksOccupied; i < blocksOccupied + 5; i++) {
        printf("FAT[%lu] (start of free space) = %d\n", i, FAT[i]);
    }
    for (uint64_t i = numBlocks - 5; i < numBlocks; i++) {
        printf("FAT[%lu] (end of FAT) = %d\n", i, FAT[i]);
    }

    if (LBAwrite(FAT, blocksOccupied, vcb->fatLoc) != blocksOccupied) {
        fprintf(stderr, "Error: Failed to write FAT to disk.\n");
        free(FAT);
        return -1;
    }

    int *verifyFAT = (int *)malloc(numBlocks * sizeof(int));
    if (verifyFAT != NULL) {
        LBAread(verifyFAT, blocksOccupied, vcb->fatLoc);
        uint64_t lastFreeBlock = blocksOccupied + vcb->freeBlockCount - 1;
        printf("Post-write EOF marker check: Last free block (FAT[%lu]) = %d\n", 
               lastFreeBlock, 
               verifyFAT[lastFreeBlock]);
        free(verifyFAT);
    }

    printf("FAT initialized: %u free blocks, %u metadata blocks.\n",
           vcb->freeBlockCount, vcb->fatSize);

    return 0;
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
// returns -1 when there is no more space left
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
    int FirstIndex = congtigiousIndex;


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
            // 
            nextBlockChain = allocateBlock(bufferOfFat, index+1, bufferSize);
            if (nextBlockChain == -1) {

                printf("\nNo more available blocks\n");
                return -1;
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

    writeBlocks(0,bufferSize, bufferOfFat);
	return FirstIndex;
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