/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
*
* File:: fsFAT.h
*
* Description:: creates function prototypes for Freespace
* management as well as global instance of the FAT
*
**************************************************************/
#ifndef FSFAT_H
#define FSFAT_H

#ifndef uint64_t
typedef uint64_t uint64_t;
#endif

extern int* FAT;

int initFAT(uint64_t numBlocks, uint64_t lastBlock); // function to initialize the FAT
int allocateBlocks(uint64_t blockCount, uint64_t blockSize); // function to allocate blocks in memory
int allocateBlock(); // helper function for allocateBlocks to allocate a single block
int readBlocks(uint64_t rootBlock, uint64_t numBlocks, void* buffer); // helper function for FAT
int writeBlocks(uint64_t rootBlock, uint64_t numBlocks, void* buffer); // helper function for FAT
void debugPrintFAT(); // used for debugging
void verifyFreeSpaceChain(); // used for debugging

#endif