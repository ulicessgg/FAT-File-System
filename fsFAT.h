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

void initFAT(uint64_t numberOfBlocks);
int allocateBlock();
int allocateBlocks(uint64_t blockCount, uint64_t blockSize);

#endif