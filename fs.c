#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include "disk.h"

#define SUPER_BLOCK 0
#define STRING_LEN 16
#define INODE_NUMBER 64
#define FILE_DESCRIPTOR 32
#define FILE_NUM 64

enum file_type {
  FILE_TYPE,
  DIRECTORY
};

typedef struct dir_entry {
  bool is_used;
  int inode_number;
  char name[STRING_LEN]; 
} dir;

typedef struct super_block {
  int empty;
} god;

typedef struct file_descripter{
  bool is_used;
  int inode_num;
  unsigned int offset;
} fd;

struct inode {
  enum file_type FT;
  unsigned int ref_count;



};

int bit_ext(uint16_t num, int k, int p) {
  return (((1 << k) - 1) & (num >> (p - 1)));
}

int modifyBit(int n, int p, int b)
{
    int mask = 1 << p;
    return ((n & ~mask) | (b << p));
}

int calc_off(unsigned int offset) {
  return offset / 16;
}

static unsigned int fd_num_used;
static uint16_t block_list[DISK_BLOCKS / 16];
static uint16_t inode_list[INODE_NUMBER / 4];
//static fd fd_list[FILE_DESCRIPTOR];
//static struct inode inode_list[INODE_NUMBER];

int make_fs(const char *disk_name) {
  if(make_disk(disk_name) != 0) {
    perror("ERROR: Failed to create disk for FS");
    return -1;
  }
  if(open_disk(disk_name) != 0) {
    perror("ERROR: Disk could not be opened");
    return -1;
  }
  fd_num_used = 0;
  for (int i = 0; i < (DISK_BLOCKS/16); i++) {
    block_list[i] = 0;
  }
  for (int i = 0; i < (INODE_NUMBER / 4); i++) {
    inode_list[i] = 0;
  }
  //inode_list[SUPER_BLOCK];
  //char *inode_bitmap;
  //char *block_bitmap

  return 0;
}
int mount_fs(const char *disk_name) {
  return 0;
}
int umount_fs(const char *disk_name) {
  return 0;
}
int fs_open(const char *name) {
  return 0;
}
int fs_close(int fildes) {
  return 0;
}
int fs_create(const char *name) {
  return 0;
}
int fs_delete(const char *name) {
  return 0;
}
int fs_read(int fildes, void *buf, size_t nbyte) {
  return 0;
}
int fs_write(int fildes, void *buf, size_t nbyte) {
  return 0;
}
int fs_get_filesize(int fildes) {
  return 0;
}
int fs_listfiles(char ***files) {
  return 0;
}
int fs_lseek(int fildes, off_t offset) {
  return 0;
}
int fs_truncate(int fildes, off_t length) {
  return 0;
}