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
#ifndef PARSEPATH_H
#define PARSEPATH_H

#include "fsDirEnt.h"
#include "fsLow.h"
#include "mfs.h"
#include "fsFAT.h"	// ensure this is always present!
#include "fsVCB.h"


extern dir_Entry *root

// Function prototypes
int parsePath(char *path, dir_Entry *returnParent, int *index, char **lastElement);
int findInDir(dir_Entry *parent, char *name);

#endif 
