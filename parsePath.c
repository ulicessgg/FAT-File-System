/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit 
* Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: parsePath.c
*
* Description:: implements the function that parses the 
* directory path given by the user. 
* 
*
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsFAT.h"	// ensure this is always present!
#include "fsVCB.h"


int parsePath(char * path, dir_Entry * returnParent, int index, char ** lastElement)
{

}