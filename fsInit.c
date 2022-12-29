/**************************************************************
 * Class:  CSC-415-0# Fall 2021
 * Names:
 * Student IDs:
 * GitHub Name:
 * Group Name:
 * Project: Basic File System
 *
 * File: fsInit.c
 *
 * Description: Main driver for file system assignment.
 *
 * This file is where you will start and initialize your system
 *
 **************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fsLow.h"
#include "mfs.h"
#include "dir_manager.h"
#include "fs_space_manager.h"
struct vcb *vcb_info;
directoryEntry *r; 
/*
	intiFileSystem() will first check that the "disk" does not already have data
	written to it. We will know this by loading in the Volume control block off "disk"
	into memory and checking for the Volume control block's signature.

	Init the volume control block with some default values.

*/


int init_vcb(uint64_t numberOfBlocks, uint64_t blockSize)
{
	struct vcb *vol = malloc(blockSize);
	vol->disk_size = sizeof(blockSize * numberOfBlocks);
	vol->block_size = blockSize;
	vol->block_count = numberOfBlocks;
	char name[16] = "Pseudo Warriors";
	for (int i = 0; i < 16; i++)
	{
		vol->v_name[i] = name[i];
	}

	vol->v_signature = FS_SIGNATURE;
	vol->f_start_block = 0; // 8 bytes
	vol->f_total_blocks = 0;

	vol->d_root_block = 0;	// 8 bytes
	vol->d_total_block = 0; // 8 bytes
	LBAwrite(vol, 1, 0);

	return 0;
}

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */
	struct vcb *vcbptr = malloc(blockSize);
	vcb_info = malloc(blockSize);

	LBAread(vcbptr,VCB_SIZE, VCB_POS); // read first block of volume, should be vcb

	if (vcbptr->v_signature == FS_SIGNATURE)
	{	
		// Prints existing directories
		r = malloc(MINBLOCKSIZE);
		LBAread(r,1,vcbptr->d_root_block);
		for(int i=0;i<DIR_LEN;i++){
			printf("DIR %d: %s type: %d parent %d, child %d\n ",i,r[i].name,r[i].type,r[i].parent_block,r[i].child_block);
		}
		return 0;
	}
	int vcb = init_vcb(numberOfBlocks,blockSize);
	LBAread(vcb_info,1,0);


	int freespace = init_free_space(numberOfBlocks, blockSize);
	int root_dir = init_root_dir(blockSize);
	LBAread(vcb_info,VCB_SIZE,VCB_POS);
	vcb_info->d_root_block = root_dir;
	vcb_info->d_total_block = ((sizeof(directoryEntry))*DIR_LEN + (blockSize-1)) / (blockSize);
	LBAwrite(vcb_info,1,0);
	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}
