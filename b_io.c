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
#include "parsePath.h"

#define MAXFCBS 20
#define B_CHUNK_SIZE 512

typedef struct b_fcb
{
	/** TODO add al the information you need in the file control block **/
	dir_Entry* fi; // directory entry info for current file
	char * buffer;		//holds the open file buffer
	int index;		//holds the current position in the buffer 
	int bufferUsed;		//holds how many valid bytes are in the buffer
	int blockPosition; // holds the begining block poisiton
	int dirPosition; // holds position in the parent directory
	int totalBlocks; // holds the total blocks taken up by file
	int flags; // holds flags modifiying file actions
} b_fcb;
	
b_fcb fcbArray[MAXFCBS];

int startup = 0;	//Indicates that this has not been initialized

//Method to initialize our file system
void b_init ()
{
	if(startup > 0) // already initialized
	{
		return;
	}

	for (int i = 0; i < MAXFCBS; i++)
	{
		//indicates a free fcbArray
		fcbArray[i].buffer = NULL;
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
			fcbArray[i].fi = (dir_Entry*) -2;
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
	// if found return the index if not create the file and return the index

	// call parsepath somewhere here in order to get the directory entry for the file

	//*** TODO ***:  Modify to save or set any information needed
	//
	//
		
	if (startup == 0)  //Initialize our system
	{
		b_init();
	}
	
	// create file descriptor using return value of b_getFCB
	b_io_fd fd = b_getFCB();
	// if b_getFCB returns -1 return a negative
	if(fd < 0)
	{
		return -2;
	}

	int* index = NULL;
	char* lastElement = NULL;

	// save the file info returned from GetFileInfo
	parsePath(filename, &fcbArray[fd].fi, &index, &lastElement);
	// if GetFileInfo returns null return a negative
	if(fcbArray[fd].fi == NULL)
	{
		return -1;
	}

	// allocate buffer for respective file being opened
	fcbArray[fd].buffer = malloc(B_CHUNK_SIZE);
	// return a negative if the allocation fails
	if(fcbArray[fd].buffer == NULL)
	{
		return -999;
	}

	// clear the fcbArray members before read
	fcbArray[fd].index = 0;
	fcbArray[fd].bufferUsed = 0;
	fcbArray[fd].blockPosition = 0;
	fcbArray[fd].dirPosition = fcbArray[fd].fi->blockPos;
	fcbArray[fd].totalBlocks = (fcbArray[fd].fi->size + (B_CHUNK_SIZE - 1)) 
							   / B_CHUNK_SIZE;
	// set the flags for the current file
	fcbArray[fd].flags = flags;

	return fd;
}

// if time allows this will be done after write due to lower priority
// due to issues with other functions, partly due to them being neglected
// or half hazardly done seek will be skipped in order to have time to debug
// and comment out dangerous code from mfs 

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


// to be done after read is finished as this will take longest time
// Interface to write function	
int b_write (b_io_fd fd, char * buffer, int count)
{
	int bytesCopied = 0; // used for write
	char* tempBuf = malloc(B_CHUNK_SIZE); // used to copy to file buffer
	fcbArray[fd].buffer; // used to write

	if (startup == 0) //Initializes our system
	{
		b_init(); 
	}

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
	{
		return (-1); 					//invalid file descriptor
	}

	// and check that the specified FCB is actually in use	
	if (fcbArray[fd].fi == NULL)		//File not open for this descriptor
	{
		return -1;
	}

    // initializes the value of tempbuf with 1 block of data for write
    strncpy(tempBuf, buffer, B_CHUNK_SIZE);

    // copies and writes buffer to memory
    while(tempBuf != NULL)
    {
        // spaceNeeded is set to the total bytes of temp
        int spaceNeeded = strlen(tempBuf);

        // if buffer is near capacity  
        if(fcbArray[fd].index + spaceNeeded >= B_CHUNK_SIZE)
        {
            // as much of temp is copied and the buffer is commited
            memcpy(fcbArray[fd].buffer + fcbArray[fd].index, tempBuf, 
				   (B_CHUNK_SIZE - fcbArray[fd].index));

			// strung together version of commitBlock to write to disk
			int freeBlock = allocateBlocks(1, 1);
			LBAwrite(fcbArray[fd].buffer, 1, freeBlock);

            // temp is moved ahead after the copied contents for next write
            tempBuf += (B_CHUNK_SIZE - fcbArray[fd].index);

			// bytesCopied is used to return the total memory written
			bytesCopied += spaceNeeded; 

            // spaceNeeded and index are updated for next write
            spaceNeeded -= (B_CHUNK_SIZE - fcbArray[fd].index);
            fcbArray[fd].index = 0;
        }

        // contents of temp are copied and index is updated
        memcpy(fcbArray[fd].buffer + fcbArray[fd].index, tempBuf, spaceNeeded);
        fcbArray[fd].index += spaceNeeded;

        // temp is set to the next string block
        strncpy(tempBuf, buffer + bytesCopied, B_CHUNK_SIZE);
    }

    // if any strings are leftover the buffer will be commited
    if(fcbArray[fd].index > 0)
    {
        // strung together version of commitBlock to write to disk
		int freeBlock = allocateBlocks(1, 1);
		LBAwrite(fcbArray[fd].buffer, 1, freeBlock);
    }

    // tempBuf is deallocated
    free(tempBuf);
		
	return bytesCopied;
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
	int bytesRead = 0; // for reading
	int bytesCopied = 0; // value returned at the end of read
	int part1 = 0, part2 = 0, part3 = 0; // holds copy lengths for three cases being handled
	int blocksToCopy = 0; // holds number of whole blocks that are needed
	int remainingBytes = 0; // holds bytes within the buffer

	if (startup == 0) //Initialize our system
	{
		b_init(); 
	}

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
	{
		return (-1); 					//invalid file descriptor
	}

	// and check that the specified FCB is actually in use	
	if (fcbArray[fd].fi == NULL)		//File not open for this descriptor
	{
		return -1;
	}

	// flag handling will go here once done with the main portions of the function

	// get the current number of bytes available in the buffer
	remainingBytes = fcbArray[fd].bufferUsed -fcbArray[fd].index;
	
	// limit count to file length to account for eof
	int amountDelivered = (fcbArray[fd].blockPosition * B_CHUNK_SIZE) - remainingBytes;
	if((count + amountDelivered) > fcbArray[fd].fi->size)
	{
		count = fcbArray[fd].fi->size - amountDelivered;
		if(count < 0)
		{
			printf("ERROR INVALID COUNT");
		}
	}

	// part 1, copy from the current buffer
	if(remainingBytes >= count)
	{
		part1 = count;
		part2 = 0;
		part3 = 0;
	}
	else
	{
		part1 = remainingBytes; // first read
		part3 = count - remainingBytes; // part1 not enough handles excess

		// calculate how many byte chunks needed
		blocksToCopy = part3 / B_CHUNK_SIZE;
		part2 = blocksToCopy * B_CHUNK_SIZE;

		// adjust part3 by the number of bytes copied in chunks
		part3 = part3 - part2;
	}

	if(part1 > 0) // memcpy part 1
	{
		memcpy(buffer, fcbArray[fd].buffer + fcbArray[fd].index, part1);
		fcbArray[fd].index = fcbArray[fd].index - part1;
	}
	if(part2 > 0) // copy directly to user buffer
	{
		bytesRead = LBAread(buffer + part1, blocksToCopy, fcbArray[fd].blockPosition + fcbArray[fd].fi->blockPos);
		fcbArray[fd].blockPosition += blocksToCopy;
		part2 = bytesRead * B_CHUNK_SIZE;
	}
	if(part3 > 0) // refill buffer to copy more
	{
		bytesRead = LBAread(fcbArray[fd].buffer, 1, fcbArray[fd].blockPosition + fcbArray[fd].fi->blockPos);
		bytesRead = bytesRead * B_CHUNK_SIZE;

		fcbArray[fd].blockPosition += 1;

		// reset the offset and buffer length
		fcbArray[fd].index = 0;
		fcbArray[fd].bufferUsed = bytesRead; // actual bytes in buffer

		if(bytesRead < part3) // not enough for request
		{
			part3 = bytesRead;
		}
		if(part3 > 0)
		{
			memcpy(buffer + part1 + part2, fcbArray[fd].buffer + fcbArray[fd].index, part3);
			fcbArray[fd].index = fcbArray[fd].index + part3; // adjust for copied bytes
		}
	}

	bytesCopied = part1 + part2 + part3;
		
	return bytesCopied; // changed it
}
	
// Interface to Close the file	
int b_close (b_io_fd fd)
{
	// deallocate fileinfo and buffer for respective file
	free(fcbArray[fd].fi);
	free(fcbArray[fd].buffer);

	// clear the fcbArray members before closing
	fcbArray[fd].index = 0;
	fcbArray[fd].bufferUsed = 0;
	fcbArray[fd].blockPosition = 0;
	fcbArray[fd].dirPosition = 0;
	fcbArray[fd].totalBlocks = 0;
	fcbArray[fd].flags = 0;

	return 0;
}
