/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
*
* File:: fsOpendir.h
*
* Description::The .h file
*
*
**************************************************************/

#ifndef FSOPENDIR_H
#define FSOPENDIR_H

// Struct representing an open directory
typedef struct fdDir {
    int currentEntry;            /* Current entry index */
    int totalEntries;            /* Total number of entries in the directory */
    struct fs_diriteminfo *entries; /* Array of directory entries */
} fdDir;

// Function prototype for opening a directory
fdDir* fs_opendir(const char *pathname);

#endif 

