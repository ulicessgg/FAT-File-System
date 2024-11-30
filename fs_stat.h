/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
*
* File:: fs_stat.h
*
* Description:: Header file for fs_stat function
**************************************************************/

#ifndef FS_STAT_H
#define FS_STAT_H

#include "fsDirEnt.h"

// File type definitions
#define FS_TYPE_FILE 1
#define FS_TYPE_DIR  2

// Structure to hold file/directory metadata
struct fs_stat {
    int type;          // File type: FS_TYPE_FILE or FS_TYPE_DIR
    unsigned int size; // Size in bytes
};

// Function prototype for fs_stat
int fs_stat(const char *path, struct fs_stat *buf);

#endif // FS_STAT_H
