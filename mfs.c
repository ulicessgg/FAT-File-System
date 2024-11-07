/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: mfs.h
*
* Description:: 
*	This is the file system interface.
*	This is the interface needed by the driver to interact with
*	your filesystem.
*
**************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "b_io.h"
#include "fsDirEnt.h"
#include "mfs.h"

// Key directory functions
int fs_mkdir(const char *pathname, mode_t mode) // marco
{
    
}

int fs_rmdir(const char *pathname); // optional

// Directory iteration functions
dir_Entry* fs_opendir(const char *pathname)
{

}
struct fs_diriteminfo *fs_readdir(fdDir *dirp)
{

}

int fs_closedir(fdDir *dirp)
{

}

// Misc directory functions
char* fs_getcwd(char *pathname, size_t size) // marco
{

}
int fs_setcwd(char *pathname)   //linux chdir // ulices
{

}
int fs_isFile(char * filename)	//return 1 if file, 0 otherwise // ulices
{

}
int fs_isDir(char * pathname)		//return 1 if directory, 0 otherwise // marco
{

}

int fs_delete(char* filename);	//removes a file // optional

int fs_stat(const char *path, struct fs_stat *buf)
{

}


