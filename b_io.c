/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
* Project:: Basic File System
*
* File:: b_io.c
*
* Description:: Basic File System - Key File I/O Operations
*
**************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>			// for malloc
#include <string.h>			// for memcpy
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "b_io.h"
#include "fsDirEnt.h"
#include "fsFAT.h"
#include "fsLow.h"
#include "fsVCB.h"
#include "mfs.h"

#define MAXFCBS 20
#define B_CHUNK_SIZE 512

typedef struct b_fcb
{
	/** TODO add al the information you need in the file control block **/
	dir_Entry* currFileInfo; // directory entry info for current file
	dir_Entry* parentDir; // directory entry info for parent directory
	char * buffer;		//holds the open file buffer
	int index;		//holds the current position in the buffer 
	int buflen;		//holds how many valid bytes are in the buffer
	int bytePosition; // holds the position in bytes in the buffer
	int blockPosition; // holds the begining block poisiton
	int dirPosition; // holds position in the parent directory
	int size; // holds the total blocks taken up by file
	int flags; // holds flags modifiying file actions
} b_fcb;
	
b_fcb fcbArray[MAXFCBS];

int startup = 0;	//Indicates that this has not been initialized

//Method to initialize our file system
void b_init ()
{
	// return if already initialized
	if(startup)
	{
		return;
	}
	
	//init fcbArray to all free
	for (int i = 0; i < MAXFCBS; i++)
	{
		fcbArray[i].currFileInfo = NULL; //indicates a free fcbArray
	}
		
	startup = 1;
}

//Method to get a free FCB element
b_io_fd b_getFCB ()
{
	for (int i = 0; i < MAXFCBS; i++)
	{
		if (fcbArray[i].buffer == NULL)
			{
				return i;		//Not thread safe (But do not worry about it for this assignment)
			}
	}

	return (-1);  //all in use
}
	
// Interface to open a buffered file
// Modification of interface for this assignment, flags match the Linux flags for open
// O_RDONLY, O_WRONLY, or O_RDWR
b_io_fd b_open (char * filename, int flags)
{
	b_io_fd fd;

	//*** TODO ***:  Modify to save or set any information needed
	//
	//
		
	if (startup == 0) b_init();  //Initialize our system
	
	fd = b_getFCB();				// get our own file descriptor
										// check for error - all used FCB's

	if(fd == -1)
	{
		return -1;
	}

	fcbArray[fd].currFileInfo = GetFileInfo(filename);
	
	return (fd);						// all set
}


// Interface to seek function	
int b_seek (b_io_fd fd, off_t offset, int whence)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
		
	return (0); //Change this
	}



// Interface to write function	
int b_write (b_io_fd fd, char * buffer, int count)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
		
	return (0); //Change this
	}



// Interface to read a buffer

// Filling the callers request is broken into three parts
// Part 1 is what can be filled from the current buffer, which may or may not be enough
// Part 2 is after using what was left in our buffer there is still 1 or more block
//        size chunks needed to fill the callers request.  This represents the number of
//        bytes in multiples of the blocksize.
// Part 3 is a value less than blocksize which is what remains to copy to the callers buffer
//        after fulfilling part 1 and part 2.  This would always be filled from a refill 
//        of our buffer.
//  +-------------+------------------------------------------------+--------+
//  |             |                                                |        |
//  | filled from |  filled direct in multiples of the block size  | filled |
//  | existing    |                                                | from   |
//  | buffer      |                                                |refilled|
//  |             |                                                | buffer |
//  |             |                                                |        |
//  | Part1       |  Part 2                                        | Part3  |
//  +-------------+------------------------------------------------+--------+
int b_read (b_io_fd fd, char * buffer, int count)
{

	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
	{
		return (-1); 					//invalid file descriptor
	}
		
	return (0);	//Change this
}
	
// Interface to Close the file	
int b_close (b_io_fd fd)
{
	// deallocate fileinfo, parent info, and buffer for respective file
	free(fcbArray[fd].currFileInfo);
	free(fcbArray[fd].parentDir);
	free(fcbArray[fd].buffer);

	// reset fcbArray values as they are no longer in use
	fcbArray[fd].currFileInfo == NULL;
	fcbArray[fd].parentDir == NULL;
	fcbArray[fd].buffer == NULL;
	fcbArray[fd].index = 0;	 
	fcbArray[fd].buflen = 0;	 
	fcbArray[fd].bytePosition = 0;	 
	fcbArray[fd].blockPosition = 0;	 
	fcbArray[fd].dirPosition = 0;	  
	fcbArray[fd].size = 0;	 
	fcbArray[fd].flags = 0;	 

	return 0;
}
