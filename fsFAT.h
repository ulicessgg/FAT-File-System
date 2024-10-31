//fsFAT.h
#ifndef FS_FAT_H
#define FS_FAT_H

#include <stdint.h>

// Define a node in the FAT linked list
typedef struct FATNode {
    uint64_t blockIndex;
    struct FATNode* next;
} FATNode;

extern FATNode* freeListHead; //Declare the freeListHead variable

void initFAT(uint64_t numberOfBlocks);
void freeFAT(); //Function to free the FAT linked list

#endif //FS_FAT_H