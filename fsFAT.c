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
#include "fsFAT.h"		// ensure this is always present!

FATNode* initFAT(FATNode* freeListHead, uint64_t numberOfBlocks) {
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

	LBAwrite(freeListHead, 1, 1);
    
    printf("FAT linked list initialized with %lu blocks.\n", numberOfBlocks - 1);
    
    return freeListHead;
}

FATNode* freeFAT(FATNode* freeListHead) {
    FATNode* current = freeListHead;
    while (current != NULL) {
        FATNode* temp = current;
        current = current->next;
        free(temp);
    }
    freeListHead = NULL; // Reset head pointer

    return freeListHead;
}
