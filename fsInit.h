/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit 
* Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
*
* File:: parsepath.h
*
* Description:: Parses the file path of the user input and
* helper methods to implment the .c file
**************************************************************/
#ifndef FSINIT_H
#define FSINIT_H

#include "fsDirEnt.h"
#include "fsLow.h"
#include "mfs.h"
#include "fsFAT.h"	// ensure this is always present!
#include "fsVCB.h"
#include "fsInit.h"

// Function prototypes
int parsePath(char * path, dir_Entry * returnParent, int index, char ** lastElement)

#endif 
