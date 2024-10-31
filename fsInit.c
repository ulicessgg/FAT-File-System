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
#include "fsVCB.h"
#include "fsDirEnt.h"

// Unique signature for VCB identification
uint64_t signature = 314159265358979323;

// Global instances for system-wide use
volumeControlBlock* vcb;
int* FAT;
dir_Entry* root;

void initFAT(uint64_t numberOfBlocks) {
    FAT = (int*)malloc(numberOfBlocks * sizeof(int));
    if (FAT == NULL) {
        perror("FAILED TO ALLOCATE THE FAT ARRAY");
        exit(-1);
    }
    
    // Initialize FAT with -1 (indicating all blocks are free)
    for (uint64_t i = 0; i < numberOfBlocks; i++) {
        FAT[i] = -1;
    }
    printf("FAT array initialized with %lu blocks.\n", numberOfBlocks);
}

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize) {
    printf("Initializing File System with %lu blocks and a block size of %lu\n", numberOfBlocks, blockSize);

    // Allocate and initialize the VCB
    vcb = (volumeControlBlock*)malloc(sizeof(volumeControlBlock));
    if (vcb == NULL) {
        perror("FAILED TO ALLOCATE THE VCB");
        exit(-1);
    }

    // Load VCB from disk
    LBAread(vcb, 1, 0);

    // If VCB is already initialized
    if (vcb->signature == signature) {
        printf("Volume Control Block Present!\n");

        // Load FAT and root directory from disk
        FAT = (int*)malloc(vcb->freeBlockCount * sizeof(int));
        if (FAT == NULL) {
            perror("FAILED TO LOAD FAT");
            exit(-1);
        }
        LBAread(FAT, vcb->freeBlockCount, vcb->freeBlockStart);

        root = (dir_Entry*)malloc(blockSize);
        if (root == NULL) {
            perror("FAILED TO LOAD ROOT");
            free(FAT);
            exit(-1);
        }
        LBAread(root, 1, vcb->rootLoc);

    } else {
        printf("Volume Control Block not Present! Initializing new file system.\n");

        // Initialize the FAT array
        initFAT(numberOfBlocks);

        // Allocate and initialize root directory entry
        root = (dir_Entry*)malloc(blockSize);
        if (root == NULL) {
            perror("FAILED TO ALLOCATE ROOT");
            free(FAT);
            exit(-1);
        }

        // Initialize VCB fields
        vcb->totalBlocks = numberOfBlocks;
        vcb->blockSize = blockSize;
        vcb->freeBlockCount = numberOfBlocks - 1; // Reserve one block for the VCB
        vcb->freeBlockStart = 1; // FAT starts after the VCB
        vcb->fatSize = numberOfBlocks - 1;
        vcb->rootLoc = 2; // Example location for the root directory
        vcb->signature = signature;
        strcpy(vcb->sysType, "File Allocation Table");

        // Write initialized VCB, FAT, and root directory to disk
        LBAwrite(vcb, 1, 0);
        LBAwrite(FAT, numberOfBlocks - 1, 1);
        LBAwrite(root, 1, vcb->rootLoc);

        printf("Finished initializing VCB, FAT, and root directory.\n");
    }

    return 0;
}

void exitFileSystem() {
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
