/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
*
* File:: fsReadDir.h
*
* Description:: Header file for fsReadDir.c
**************************************************************/

#ifndef FSREADDIR_H
#define FSREADDIR_H

#include "fsDirEnt.h" // Directory entry definitions
#include "fsFAT.h"    // FAT-related functions
#include "fsVCB.h"    // Virtual Control Block definitions

// Function prototype for reading and printing directory entries
int readdir(const char *dirPath);

#endif // FSREADDIR_H


// helper function, here's the directory entry i want to load, i need to allocate size and round it up to the next block size and call helper funciton in fat to read chain
// start from beginning, read HEAD from block 0 to block n and the buffer. this will go block by block loading each of the blocks into your buffer (the directory)
// remember to increment the pointer when loading blocks so that we don't keep loading into the same spot in memory