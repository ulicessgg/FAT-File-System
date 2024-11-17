/**************************************************************
* Class::  CSC-415-01 Fall 2024
* Name:: Ulices Gonzalez, Marco Robles, Yash Pachori, Prashrit 
* Magar
* Student IDs:: 923328897, 921282632, 923043313, 922068027
* GitHub-Name:: csc415-filesystem-ulicessgg
* Group-Name:: The Gunners
*
* File:: fsDirEnt.h
*
* Description:: creates the directory entry struct and helper
* function prototypes to implement in the .c file
*
**************************************************************/
#ifndef FSDIRENT_H
#define FSDIRENT_H


#ifndef time_t
typedef time_t time_t;
#endif

typedef struct dir_Entry
{
    time_t creation_date; // Saves the creation timestamp, 8 bytes
    time_t last_edited; // When edited saves the timestamp, 8 bytes
    time_t last_accessed; // When accessed saves the timestamp, 8 bytes
    unsigned int blockPos; // Holds index of the entry in blocks, 4 bytes
    unsigned int size; // Stores the size in bytes of the file, 4 bytes
    int is_Directory; // Identifies a directory or file, 4 byte
    char name[60]; // Name of the file or directory being created, 60 bytes
} dir_Entry;

dir_Entry* createDirectory(int NumofEntries, dir_Entry* parent) ; // refer to 10/15 and 10/17 lecture to implement

#endif