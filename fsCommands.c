/**************************************************************
* Class:  CSC-415-02 Summer 2021
* Names: Emanuel Francis,Christian Jackson, Anish Somisetty, Johnny Tram
* GitHub Name: Emanuelf-sfsu, narxlol, vsomisetty1, JohnnyTram
* Group Name:Pseudo Warriors
* Project: Basic File System
*
* File: fsCommands.h
*
* Description: This program will run the commands listed in fsshell.c
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mfs.h>
#include <fsLow.h>
#include <dir_manager.h>
#include <fs_space_manager.h>
#define DIRMAX_LEN 4096
int valid_path(char *path);
void printALL();
int find_empty(struct directoryEntry *start, int max_entries);


struct directoryEntry *directory;
struct directoryEntry *c_dir;
extern struct vcb *vcb_info;
int root = 0;
int cwd = 0;

struct path_info
{
  /* data */
  char *path;
  char *argv[255];
  char *first;
  char *last;
  int path_len;
};
/*
  Strtok does not like '/' and we are not sure how to fix it so
  we will run through the path searching for '/' first and replace them
  with spaces. Then run the path through strtok and store some information
  about the path in a struct that could be useful of our other functions.
*/
struct path_info parse_path(char *path)
{
  char delim[2] = " ";
  char *token;
  char *s1 = NULL;
  int args_count = 0;
  struct path_info ptr;

  if (path[0] == '/')
  {
    ptr.first = "/";
  }

  char tmp[strlen(path)];
  for (int i = 0; i < strlen(path); i++)
  {
    if (path[i] != '/')
    {
      tmp[i] = path[i];
    }
    else
    {
      tmp[i] = ' ';
    }
  }

  char *use = malloc(sizeof(tmp));
  strcpy(use, tmp);
  token = strtok_r(use, delim, &s1);

  while (token != NULL)
  {

    ptr.argv[args_count] = token;
    args_count++;

    token = strtok_r(NULL, delim, &s1);
  }

  ptr.first = ptr.argv[0];
  ptr.path_len = args_count;
  if (ptr.path_len == 1)
  {
    ptr.last = ptr.argv[0];
    return ptr;
  }

  ptr.last = ptr.argv[args_count - 1];
  free(use);
  return ptr;
}

/*
  Load the current directory
  1.  Does the path exist in the directory?
  2.  If so create a sub directory with the parents block and the childs block numbers
      The sub directory entries will be in a free state.
*/
int fs_mkdir(const char *pathname, mode_t mode)
{
  vcb_info = malloc(MINBLOCKSIZE);
  LBAread(vcb_info, VCB_SIZE ,VCB_POS);
  char buf[DIRMAX_LEN];
  // printf("\nCreate: %s\n",pathname);
  strcpy(buf, pathname);
  struct path_info path = parse_path(buf);
  // if(path.first != "/"){
  //   return 1;
  // }
  cwd = vcb_info->d_root_block;
  directory = malloc(vcb_info->d_total_block * MINBLOCKSIZE);
  LBAread(directory, vcb_info->d_total_block, vcb_info->d_root_block);

  int result = find(directory,path.last, DIR_LEN);
  if (result == -1)
  {
    for (int i = 0; i < DIR_LEN; i++)
    {
      if (directory[i].type == FREE)
      {
        char* name = path.last;
        memcpy(directory[i].name,name, strlen(name));
        directory[i].type = DIR_;
        int new_child = create_dir(directory[0].child_block);
        directory[i].child_block = new_child;
        directory[i].parent_block = directory[0].parent_block;
        LBAwrite(directory, 1, cwd);
        return 1;
      }
    }
  }

  // if(cwd == 0){
  //   c_dir = malloc(MINBLOCKSIZE);
  //   LBAread(c_dir,vcb_info->d_total_block,vcb_info->d_root_block);
  // }
  //   printf("Root location %s\n",c_dir[0].name);

  // for(int i = 0; i<DIR_LEN; i++){
  //     printf("Found\n");

  //   if(c_dir[i].type == FREE){
  //     memmove(c_dir[i].name,path.last,sizeof(c_dir[i].name));
  //     printf("Found\n");
  //     create_dir(c_dir[i]);

  //     return 1;
  //   }
  // return 1;
  // }

  // printf("ROOT block %d\n", vcb_info->d_root_block);

  // int d_start, d_total;
  // char* temp = pathname;
  // struct path_info path = parse_path(temp);
  // vcb_info = malloc(MINBLOCKSIZE);
  // LBAread(vcb_info, 1, 0);

  // d_total = vcb_info->d_total_block;
  // d_start = vcb_info->d_root_block;
  // directory = malloc(sizeof(d_total * MINBLOCKSIZE));
  // LBAread(directory, d_total, d_start);

  // char* new_dir = path.last;
  // int index =0;
  // while(1){
  //   if(directory[index].type == FREE)
  //   {
  //     memmove(directory[index].name,path.last,sizeof(directory[index].name));
  //   }
  // }

  // create_dir(c_dir[0]);
  return -1;
}

fdDir *fs_opendir(const char *name)
{
  // struct directoryEntry *cur_dir = malloc(MINBLOCKSIZE * 6);
  fdDir *ret = malloc(sizeof(fdDir));
  // if (cwd == 0)
  // {
  // directory = malloc(MINBLOCKSIZE * vcb_info->d_total_block);
  //   c_dir = malloc(MINBLOCKSIZE * vcb_info->d_total_block);
  // LBAread(directory, vcb_info->d_total_block, vcb_info->d_root_block);
  ret->directoryStartLocation = vcb_info->d_root_block;
  ret->dirEntryPosition = 0;
  ret->d_reclen = vcb_info->block_size;
  //   for (int i = 0; i < DIR_LEN; i++)
  //   {
  //     if (strcmp(directory[i].name, name))
  //     {
  //       ret->directoryStartLocation = directory[i].parent_block;
  //       ret->dirEntryPosition = i;
  //       ret->d_reclen = directory[i].parent_block;
  //       break;
  //     }
  //   }
  // }

  // return ret;
  return ret;
}

int next = 0;
struct fs_diriteminfo *fs_readdir(fdDir *dirp)
{
  vcb_info = malloc(MINBLOCKSIZE);
  LBAread(vcb_info, VCB_SIZE, VCB_POS);
  directory = malloc(MINBLOCKSIZE);
  LBAread(directory, 1, vcb_info->d_root_block);

  struct fs_diriteminfo *ret = malloc(sizeof(struct fs_diriteminfo));
  if (directory[next].type == FREE)
  {
    ret = NULL;
    next = 0;
    return ret;
  }

  strcpy(ret->d_name, directory[next].name);
  ret->fileType = directory[next].type;
  ret->d_reclen = 1;
  next++;
  return ret;
}

int fs_closedir(fdDir *dirp)
{
  if (dirp == NULL)
  {
    printf("[ERROR]\n");
    return -1;
  }
  free(dirp);
  dirp = NULL;
  return 1;
}

char *fs_getcwd(char *buf, size_t size)
{
  vcb_info = malloc(MINBLOCKSIZE);
  char path[DIRMAX_LEN];
  path[0] = '/';
  memmove(buf, path, sizeof(path));

  // if (cwd == 0)
  // {
  //   directory = malloc(MINBLOCKSIZE * vcb_info->d_total_block);
  //   c_dir = malloc(MINBLOCKSIZE * vcb_info->d_total_block);
  //   LBAread(directory, vcb_info->d_total_block, vcb_info->d_root_block);
  //   c_dir = directory;
  //   memmove(buf, path, sizeof(path));
  //   return buf;
  // }
  return buf;
}

// /*
//   Use the global directoryEntry pointer to load the new directory
//   into the pointer.

struct directoryEntry *load_dir(struct directoryEntry entry)
{
  if (entry.type != DIR_)
  {
    printf("ERROR!!\n");
    return NULL;
  }
  struct directoryEntry *newdir = malloc(MINBLOCKSIZE);
  LBAread(newdir, entry.blocks, entry.child_block);
  // printf("LOAD: %d %d", entry.blocks, entry.child_block);
  return newdir;
}

int fs_setcwd(char *buf)
{
  struct path_info setcwd = parse_path(buf);
  vcb_info = malloc(MINBLOCKSIZE);
  LBAread(vcb_info, VCB_SIZE, VCB_POS);

  directory = malloc(MINBLOCKSIZE);
  LBAread(directory, vcb_info->d_total_block, vcb_info->d_root_block);

  if (cwd == 0)
  {
    c_dir = malloc(MINBLOCKSIZE * vcb_info->d_total_block);
    c_dir = directory;
    cwd += 1;
  }

  struct directoryEntry *start, *newStart, *active;
  start = malloc(MINBLOCKSIZE * vcb_info->d_total_block);
  newStart = malloc(MINBLOCKSIZE * vcb_info->d_total_block);
  active = malloc(MINBLOCKSIZE * vcb_info->d_total_block);

  if (setcwd.first == "/")
  {
    start = directory;
  }
  else
  {
    start = c_dir;
  }

  active = start;

  int find_index = 0;
  for (int i = 0; i < setcwd.path_len - 1; i++)
  {
    find_index = find(start, setcwd.argv[i], DIR_LEN);
    if (find_index == -1)
    {
      return -1;
    }

    if (start[find_index].type == FILE)
    {
      return -1;
    }

    newStart = load_dir(start[find_index]);
    if (active != start)
    {
      free(active);
    }
    active = newStart;
  }

  find_index = find(active, setcwd.argv[setcwd.path_len - 1], DIR_LEN);
  printf("ACTIVE %s, %s\n", active[2].name, newStart[0].name);
  LBAread(c_dir, 1, active[2].child_block);

  if (find_index < 0)
  {
    return -1;
  }

  return 0;
}

int fs_isFile(char *path)
{

  return -1;
}

int fs_isDir(char *path)
{
  return 1;
  struct path_info buf = parse_path(path);
  

  vcb_info = malloc(MINBLOCKSIZE);
  LBAread(vcb_info, 1, 0);
  // cwd = vcb_info->d_root_block;
  // int index = find(cwd, buf.first, DIR_LEN);

  // directory = malloc(vcb_info->d_total_block * vcb_info->block_size);
  // LBAread(directory, vcb_info->d_total_block, cwd);

  // if (directory[index].type == DIR_)
  // {
  //   return 0;
  // }

  // struct path_info tmp = path_info(path);
  // struct directoryEntry *dir = malloc(MINBLOCKSIZE);
  // LBAread(dir,1,7);

  // int index = 0;
  // for(int i =0;i < 5;i++){
  //   if(strcasecmp(tmp.first,dir[i].name)){
  //     if(dir[i].type == DIR_){
  //         return 1;
  //     }
  //   }
  // }
  return -1;
}

// off_t     st_size;    		/* total size, in bytes */
// blksize_t st_blksize; 		/* blocksize for file system I/O */
// blkcnt_t  st_blocks;  		/* number of 512B blocks allocated */
// time_t    st_accesstime;   	/* time of last access */
// time_t    st_modtime;   	/* time of last modification */
// time_t    st_createtime;   	/* time of last status change */

int fs_stat(const char *path, struct fs_stat *buf)
{
  time_t seconds;
  seconds = time(NULL);

  struct stat *path_stat;
  buf->st_accesstime = seconds;
  buf->st_blksize = 1;
  buf->st_blocks = 1;
  buf->st_createtime = seconds;
  buf->st_modtime = seconds;
  buf->st_size = 1;

  // path_stat = (struct stat *)buf;

  // return (stat(path, path_stat));
  return 0;
}

/*
  To remove a directory we need to find it, verify that the path exists.
  Then remove the block from the free space make and reset the values in the 
  directory.
*/
int fs_rmdir(const char *pathname)
{
  
  char* buf;
  strcpy(buf,pathname);
  vcb_info =malloc(MINBLOCKSIZE);
  LBAread(vcb_info, VCB_SIZE, VCB_POS);

  if(cwd == 0){
    cwd = vcb_info->d_root_block;
  }
  directory = malloc(MINBLOCKSIZE);
  LBAread(directory,vcb_info->d_total_block,cwd);
  int status = find(directory,buf,DIR_LEN);
  if(status == -1){
    return -1;
  }
  directory[status].type = FREE;
  strcpy(directory[status].name,"");
  free_block( directory[status].child_block);
  directory[status].child_block = 0;



  return 1;
  
}

int fs_delete(char *filename)
{
  return 1;
}

int find(struct directoryEntry *start, char *path, int max_entries)
{
  int free_spot = 0;
  for (int i = 0; i < max_entries; i++)
  {
    if (free_spot == 0 && start[i].type == FREE)
    {
      free_spot = i;
    }
    if (strncmp(start[i].name, path, sizeof(start[i].name)) == 0)
    {
      printf("FOUND: %s stat: %d\n", start[i].name, strncmp(start[i].name, path, sizeof(start[i].name)));
      return i;
    }
  }

  return -1;
}
int find_empty(struct directoryEntry *start,int max_entries)
{
  for (int i = 0; i < max_entries; i++)
  {
    if (start[i].type == FREE)
    {
      return i;
    }
  }
  return -1;
}

// int valid_path(char* path)
// {
//   struct path_info ref = parse_path(path);
//   if(cwd == 0){
//     cwd = vcb_info->d_root_block;
//   }
//   root = vcb_info->d_root_block;

//   if(ref.first != "/");
//   vcb_info = malloc(MINBLOCKSIZE);
//   LBAread(vcb_info,1,0);

//   directory = malloc(vcb_info->d_total_block);
//   c_dir = malloc(vcb_info->d_total_block);
//   int found = 0;
//   LBAread(directory,vcb_info->d_total_block,vcb_info->d_root_block);
//   for(int i =0; i<ref.path_len; i++){
//      found = find(directory,ref.argv[i],DIR_LEN);
//     if(found > 0){
//       LBAread(directory,)
//     }
//   }

//   printf("DIR 2 %s",directory[1].name);
// return 0;

// }

void printALL()
{
  directory = malloc(DIR_LEN * MINBLOCKSIZE);
  struct vcb *wo = malloc(MINBLOCKSIZE);
  LBAread(wo,VCB_SIZE, VCB_POS);
  int x = wo->d_root_block;
  int y = wo->d_total_block;
  LBAread(directory, x, y);

  for (int i = 0; i < DIR_LEN; i++)
  {
    printf("Name: %s, Parent: %ld, child %ld\n", directory[i].name, wo->d_root_block, wo->d_total_block);
  }
}