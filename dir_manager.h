/**************************************************************
* Class:  CSC-415-02 Summer 2021
* Names: Emanuel Francis,Christian Jackson, Anish Somisetty, Johnny Tram
* Student IDs: 917491268, 921804361,921956526,921229449
* GitHub Name: Emanuelf-sfsu, narxlol, vsomisetty1, JohnnyTram
* Group Name:Pseudo Warriors
* Project: Basic File System
*
* File: dir_manager.h
*
* Description: This program will help manage the directory
**************************************************************/


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "mfs.h"
#define CUR_DIR "."
#define PARENT_DIR ".."

/**********************directory entry******************************/
/*
  There were much more attributes that we want to add but did not 
  get the chance to. For now directories will contain a parent & block 
  child. The parent block should be the current directory if not in 
  the ".." directory. The child block will hold another directory.
*/
typedef struct __attribute__((__packed__)) directoryEntry
{
  int type;            // 1 byte
  uint64_t blocks;      // 8 bytes
  int parent_block;
  int child_block;
  long size;      // 8 bytes
  char name[30]; // 255 bytes
} directoryEntry;

uint64_t create_dir(int parent_block);
uint64_t init_root_dir(uint64_t blocksize);
int get_dir_block();
void init_directories();
void insert(char *name,int index, char type);
int find(struct directoryEntry* start,char* path,int max_entries);
int search_dir(struct directoryEntry *dir, char* name);
int create_entry(int index,char* entry_name);


