//fsFAT.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fsFAT.h"
#include "fsVCB.h"
#include "fsDirEnt.h"

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
    
    printf("FAT linked list initialized with %lu blocks.\n", numberOfBlocks - 1);

    return freeListHead;
}