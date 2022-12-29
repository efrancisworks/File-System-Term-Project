/**************************************************************
* Class:  CSC-415-02 Summer 2021
* Names: Emanuel Francis,Christian Jackson, Anish Somisetty, Johnny Tram
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
#include <time.h>

#include "fsLow.h"
#include "mfs.h"
#include "fs_space_manager.h"
#include "dir_manager.h"
extern struct vcb *vcb_info;
extern struct directoryEntry *c_dir;

/**************************Directory********************************/

uint64_t init_root_dir(uint64_t blocksize)
{
	/*Intialize the root dir*/
	vcb_info = malloc(blocksize);
	LBAread(vcb_info, VCB_SIZE, VCB_POS);
	int blk_req = ((sizeof(directoryEntry)) * DIR_LEN + 511) / 512;
	int root_block = vcb_info->f_free_index;

	vcb_info->d_root_block = root_block;
	vcb_info->d_total_block = blk_req;
	LBAwrite(vcb_info, VCB_SIZE, VCB_POS);

	int ret = create_dir(root_block);
	struct directoryEntry *dir = malloc(MINBLOCKSIZE);
	LBAread(dir, vcb_info->d_total_block, vcb_info->d_root_block);
	dir[0].type = ROOT;
	dir[1].type = ROOT;
	LBAwrite(dir, vcb_info->d_total_block, vcb_info->d_root_block);

	// Test insert
	// char* newdir = "etc";
	// memmove(dir[2].name,newdir,sizeof(newdir));
	// int child = create_dir(dir[2].parent_block);
	// dir[2].child_block = child;
	// LBAwrite(dir,blk_req,root_block);

	return ret;
}

/*
	To create a new directory
	1. you need the parent directories block number
	2. find a new block for the child directory and save that as child directory's block number.
	Any time you want to enter a child directory, LBAread the child directory's block
	3. In all new directory entries there will be a "." and ".." index that will help use 
	return to the parent directory quickly.


*/
uint64_t create_dir(int parent_block)
{
	struct directoryEntry dir[DIR_LEN];
	for (int i = 0; i < DIR_LEN; i++)
	{
		dir[i].name[i] = '\0';
		dir[i].parent_block = vcb_info->f_free_index;
		dir[i].child_block = 0;
		dir[i].type = FREE;
		dir[i].size = sizeof(struct directoryEntry);
	}

	memmove(dir[0].name, CUR_DIR, sizeof(CUR_DIR));
	dir[0].parent_block = vcb_info->f_free_index;
	dir[0].child_block = vcb_info->f_free_index;
	dir[0].type = FREE;

	memmove(dir[1].name, PARENT_DIR, sizeof(PARENT_DIR));
	dir[1].parent_block = parent_block;
	dir[1].child_block = parent_block;
	dir[1].type = DIR_;

	int blk_req = ((sizeof(directoryEntry)) * DIR_LEN + 511) / 512;

	int ret = insert_block(dir, blk_req);

	return ret;
}

int search_dir(struct directoryEntry *dir, char *name)
{
	for (int i = 0; i < DIR_LEN; i++)
	{
		if (strcmp(dir[i].name, name))
		{
			return i;
		}
	}
	return -1;
}
