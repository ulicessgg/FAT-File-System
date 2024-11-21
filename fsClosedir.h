/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
*
* File:: fsClosedir.h
*
* Description:: the .h file
*
*
**************************************************************/

#ifndef FSCLOSEDIR_H
#define FSCLOSEDIR_H

#include "fsOpendir.h" // Include fdDir definition

// Function prototype for closing a directory
int fs_closedir(fdDir *dirp);

#endif // FSCLOSEDIR_H
