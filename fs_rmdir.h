/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit 
* Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: fs_rmdir.h
*
* Description:: header file for the fs_rmdir function
* 
* 
*
**************************************************************/

#ifndef FS_RMDIR_H
#define FS_RMDIR_H

// Removes an empty directory at the specified path.
// Returns 0 on success, -1 on failure.
int fs_rmdir(const char *dir_path);

#endif // FS_RMDIR_H
