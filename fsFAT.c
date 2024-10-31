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
#include "fsFAT.h"	// ensure this is always present!


void initFAT(uint64_t numberOfBlocks, uint64_t blockSize) {
    FAT = malloc(numberOfBlocks * sizeof(int));
    if (FAT == NULL) {
        perror("FAILED TO ALLOCATE THE FAT ARRAY");
		free(FAT);
        exit(-1);
    }
    
    // Initialize FAT with -1 (indicating all blocks are free)
    for (uint64_t i = 0; i < numberOfBlocks; i++) {
        FAT[i] = 1;
    }
	// Set the first value in the FAT as -1 to mark the VCB location
	FAT[0] = -1;
    printf("FAT array initialized with %lu blocks.\n", numberOfBlocks);
}