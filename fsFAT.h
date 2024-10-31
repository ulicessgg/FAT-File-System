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
#ifndef FSFAT_H
#define FSFAT_H

#ifndef uint64_t
typedef uint64_t uint64_t;
#endif

// Define a node in the FAT linked list
typedef struct FATNode 
{
    uint64_t blockIndex;
    struct FATNode* next;
} FATNode;

FATNode* initFAT(FATNode* freeListHead, uint64_t numberOfBlocks);
FATNode* freeFAT(FATNode* freeListHead); //Function to free the FAT linked list

#endif