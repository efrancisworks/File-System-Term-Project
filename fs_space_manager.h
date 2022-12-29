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
* Description: This program will help manage the freespace in the volume
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "mfs.h"



#define FREESIG 0x65657266

typedef struct __attribute__((__packed__)) free_space{
	uint64_t f_sig;
	unsigned char fs_bitmap[];
}free_space;

uint64_t init_free_space(uint64_t numberOfBlocks, uint64_t blockSize);
uint64_t next_free_block();
uint64_t insert_block(void *data, int num_of_blocks);
uint64_t free_block(int block_pos);

