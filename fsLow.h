/**************************************************************
 * Class:  CSC-415
 * Name: Professor Bierman
 * Student ID: N/A
 * Project: Basic File System
 *
 * File: fsLow.h
 *
 * Description: This file provides the ability to read and write
 *   Logical Blocks and is the main interface for the file system
 *   project.
 *   Note that the layer uses one more block than that presented
 *   to the file organization module.  This block is used to
 *   hold the partition information and is not accessible from
 *   any other layer.  But, when you use the hexdump utility
 *   you will see that the first block is not part of the volume.
 *
 *   The file created by this layer represents the physical hard
 *	drive.  It presents to the logical layer (your layer) as just
 *	a logical block array (a series of blocks - nominally 512 bytes,
 *	that the logical layer can utilize).
 *
 *	It is imperative that the logical layer (your layer) first
 *	call startPartitionSystem before using any function and when
 *	finished calls closePartitionSystem() to ensure that the
 *	file that represents the physical drive is properally closed.
 *
 **************************************************************/
//
// Start Partition System
//
// This is the first function to call before your filesystem starts
// If the filename already exists, then the input values stored in
// volSize and blockSize are ignored.  If the file does not exist, it will
// be created to the specified volume size in units of the block size
// (must be power of 2) plus one for the partition header.
//
// On return
// 		return value 0 = success;
//		return value -1 = file exists but can not open for write
//		return value -2 = insufficient space for the volume
//		volSize will be filled with the volume size
//		blockSize will be filled with the block size
#ifndef uint64_t
typedef u_int64_t uint64_t;
#endif
#ifndef uint32_t
typedef u_int32_t uint32_t;
#endif
typedef unsigned long long ull_t;

int startPartitionSystem(char *filename, uint64_t *volSize, uint64_t *blockSize);

int closePartitionSystem();

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize);
void exitFileSystem();

uint64_t LBAwrite(void *buffer, uint64_t lbaCount, uint64_t lbaPosition);

uint64_t LBAread(void *buffer, uint64_t lbaCount, uint64_t lbaPosition);

#define MINBLOCKSIZE 512
#define PART_SIGNATURE 0x526F626572742042
#define PART_SIGNATURE2 0x4220747265626F52
#define PART_CAPTION "CSC-415 - Operating Systems File System Partition Header\n\n"

#define PART_ACTIVE 1
#define PART_INACTIVE 0

#define PART_NOERROR 0
#define PART_ERR_INVALID -4

#define FS_SIGNATURE 0x74726174736c6f76

#define DIR_ 1
#define FILE 2
#define FREE 0
#define EXT 0xF
#define ROOT 0x746F6F
#define DIR_LEN 7
#define VCB_POS 0
#define VCB_SIZE 1

/******************************VCB**********************************/
typedef struct __attribute__((__packed__)) vcb
{
  // 285 bytes
  unsigned long disk_size;
  uint64_t block_size;
  uint64_t block_count;
  
  uint64_t v_block_size;
  unsigned long v_signature;

  // Store free space map in memory
  uint64_t f_start_block; // 8 bytes
  uint64_t f_total_blocks;
  uint64_t f_free_index;


  uint64_t d_root_block;  // 8 bytes
  uint64_t d_total_block; // 8 bytes
  char v_name[255]; // 255 bytes

} vcb;

/*This variables will help the fs shell move throughout the directories */

extern struct vcb *vcb_info;
extern struct free_space *fs_array;
extern struct directoryEntry *c_dir;

int update_globals();
int getBlksize();


