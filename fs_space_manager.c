/**************************************************************
* Class:  CSC-415-02 Summer 2021
* Names: Emanuel Francis,Christian Jackson, Anish Somisetty, Johnny Tram
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

#include "mfs.h"
#include "fsLow.h"
#include "fs_space_manager.h"
#define SET_BIT(BF, N) BF |= ((uint64_t)0x00000001 << N)
#define CLR_BIT(BF, N) BF &= ~((uint64_t)0x00000001 << N)
#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)
extern struct vcb *vcb_info;
struct free_space *fs_array;
int blk_sze = 0;
/*
    Access the freespace map using LBA read to get the
    freespace map and loop through the bit map to find the
    next free space.
*/
struct vcb *update_vcb()
{
    vcb_info = malloc(MINBLOCKSIZE);
    LBAread(vcb_info,1,0);
    return vcb_info;
}

uint64_t init_free_space(uint64_t numberOfBlocks, uint64_t blockSize)
{
    blk_sze = blockSize;
    vcb_info = malloc(blockSize);
    struct free_space *fs_array = malloc(blockSize * numberOfBlocks);
    fs_array->f_sig = FREESIG;
    int fs_blocks = ((numberOfBlocks + blockSize - 1) / blockSize);
    unsigned char fs_bitmap[numberOfBlocks];
    memset(fs_array->fs_bitmap,0, sizeof(fs_bitmap));

    /*
        Set vcb block and the total number of blocks needed to track the 
        free blocks. 
    */
    int next_free = 1;
    for (int i = 0; i < fs_blocks; i++)
    {
        fs_bitmap[i] = 1;
        next_free = i;
    }

    memmove(fs_array->fs_bitmap,fs_bitmap,numberOfBlocks);
    int status = LBAwrite(fs_array, fs_blocks, 1);
    
    LBAread(vcb_info,1,0);
    vcb_info->f_start_block = 1;
    vcb_info->f_total_blocks = numberOfBlocks;
    vcb_info->f_free_index = next_free + 1;

    // printf("NEXT FREE BLK %ld\n",vcb_info->f_free_index);
    LBAwrite(vcb_info,1,0);
    free(fs_array);

    return status;
}

/*
    Our freespace management system is orginized in an array of
    bytes. Each byte represents a bit map. In order to make our free space contious
    we must translate the index and the specific bit that is free.

    index * sizeof(long)+ index_of_free_bit

*/
uint64_t next_free_block()
{
    struct vcb *vol = malloc(MINBLOCKSIZE);
    LBAread(vol, 1, VCB_POS);
    int old = vol->f_free_index;
    if(old > vol->f_total_blocks){
        perror("VOLUME FULL!!!\n");
        return -1;
    }

    vol->f_free_index = old;
    LBAwrite(vol,1,VCB_POS);

    return old;
    // for (int i = 1; i < 1950/8; i++)
    // {   
    //     if (fs_array->fs_bitmap[i] != 0xFF)
    //     {

    //         int group_blk = i*8;
    //         // printf("\ngroup_blk: %d\n",group_blk);

    //         int bit_set = 8 - i;
    //         int k =0;
    //         // printf("bit_set: %d\n",bit_set);
    //         for(int z = 0;z<sizeof(long);z++){
    //             if(!IS_BIT_SET(fs_array->fs_bitmap[i],i)){
    //                 k = i;
    //                 break;
    //             }
    //         }
    //         int ret = group_blk + k;
    //         // printf("\nFREESPACE INDEX: %d\n", ret);
    //         free(vol);
    //         free(fs_array);
    //         return ret;
    //     }
    // }

    // return -1;
}

/*
    Determine the size need for the block and insert the block in it new
    position. Return the number of blocks written
*/
uint64_t insert_block(void *data, int num_of_blocks)
{
    int f_start, f_total, index, start;    

    vcb_info = malloc(MINBLOCKSIZE);
    LBAread(vcb_info,VCB_SIZE, VCB_POS);

    f_start = vcb_info->f_start_block;
    f_total = vcb_info->f_total_blocks;
    index = vcb_info->f_free_index;
    start = index;
    
    fs_array = malloc((vcb_info->block_size)*f_total);
    LBAread(fs_array,f_total,f_start);

    if(index >= vcb_info->block_count){
        printf("ERROR: VOLUME FULL");
        return -1;
    }

    for(index; index < num_of_blocks;index++){
        fs_array->fs_bitmap[index] = 1;
    }
    vcb_info->f_free_index = index+1;
    LBAwrite(vcb_info,VCB_SIZE,VCB_POS);
    LBAwrite(fs_array,f_total,f_start);
    LBAwrite(data,num_of_blocks,start);

    // for(int i =0; i< sizeof(long); i++){
    //     if(!IS_BIT_SET(tmp,i)){
    //         SET_BIT(tmp, i);
    //         if(covert<1){
    //             ret = i;
    //             break;
    //         }
    // printf("\nNEW BLK ENTERED AT : %d",covert);

    //         group_blk = pos*8;
    //         bit_set = 8 - i;
    //         ret = group_blk - bit_set;
    //         break;
    //     }
    // }
    // fs_array->fs_bitmap[covert] = tmp;

    // LBAwrite(fs_array,5,1);

    // printf("\nNEW BLK ENTERED AT : %d",ret);
    // int written = LBAwrite(data, num_of_blocks, ret);

    return start;
}
uint64_t free_block(int block_pos)
{
    int f_start, f_total, index, start;    

    vcb_info = malloc(MINBLOCKSIZE);
    LBAread(vcb_info,VCB_SIZE, VCB_POS);

    index = block_pos;
    
    fs_array = malloc((vcb_info->block_size)*f_total);
    LBAread(fs_array,f_total,f_start);
    fs_array->fs_bitmap[index] = 0;

    LBAwrite(vcb_info,VCB_SIZE,VCB_POS);
    LBAwrite(fs_array,f_total,f_start);

    return block_pos;
}


int getBlksize()
{
    return blk_sze;
}