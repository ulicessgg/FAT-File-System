/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
*
* File:: fsVCB.h
*
* Description:: creates the volume control block struct
*
**************************************************************/

#ifndef uint64_t
typedef uint64_t uint64_t;
#endif

typedef struct volumeControlBlock
{
    uint64_t signature;// Signature to identify the volume // 8 bytes
    uint64_t totalBlocks; // number of blocks in the volume // 8 bytes
    uint64_t blockSize; // Size of each block in bytes // 8 bytes
    unsigned int freeBlockCount; // total free space count // 4 bytes
    unsigned int freeBlockStart; // Start index for free space // 4 bytes
    unsigned int fatSize; // size of the file allocation table // 4 bytes
    unsigned int rootLoc; // Index for the root directory // 4 bytes
    char sysType[24]; // Holds the type of volume/file system // 32 bytes
} volumeControlBlock;

extern volumeControlBlock* vcb;