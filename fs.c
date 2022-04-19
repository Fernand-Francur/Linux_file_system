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
#define BLOCK_NUM 10
#define BLOCK_SIZE 4096

enum file_type {
  FILE_TYPE,
  DIRECTORY,
  UNDEFINED
};

typedef struct dir_entry {
  bool is_used;
  int inode_number;
  char name[STRING_LEN]; 
} dir;

typedef struct super_block {
  struct inode *root_inode;
  uint16_t block_bitmap_size;
  uint16_t block_bitmap_offset;
  uint16_t inode_bitmap_size;
  uint16_t inode_bitmap_offset;
  uint16_t inode_list_offset;
  uint16_t inode_list_size;
  uint16_t data_blocks_offset;
  uint16_t used_blocks_count;
} god;

typedef struct file_descripter{
  bool is_used;
  int inode_num;
  unsigned int offset;
} fd;

struct inode {
  enum file_type FT;
  unsigned int ref_count;
  void *direct_blocks[BLOCK_NUM];
  void *indirect_blocks[BLOCK_NUM];
  unsigned int offset;
  size_t file_size;
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
static uint16_t block_bitmap[DISK_BLOCKS / 16];
static uint16_t inode_bitmap[INODE_NUMBER / 16];
static fd fd_list[FILE_DESCRIPTOR];
static struct inode inode_list[INODE_NUMBER];
static god super;

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
        for (int j = 0; j < 16; j++) {
      block_bitmap[i] = modifyBit(block_bitmap[i], j, 0);
      //printf("%d\n",bit_ext(block_bitmap[i], 16, 1));

    }
  }
  for (int i = 0; i < (INODE_NUMBER / 16); i++) {
    for (int j = 0; j < 16; j++) {
      inode_bitmap[i] = modifyBit(inode_bitmap[i], j, 0);
      //printf("%d\n",bit_ext(inode_bitmap[i], 16, 1));
    }
  }
  for (int i = 0; i < FILE_DESCRIPTOR; i++) {
    fd_list[i].is_used = false;
    fd_list[i].inode_num = -1;
    fd_list[i].offset = 0;
  }


  for (int i = 0; i < INODE_NUMBER; i++) {
    inode_list[i].FT = UNDEFINED;
    inode_list[i].ref_count = 0;
    inode_list[i].offset = 0;
    inode_list[i].file_size = 0;
  }

  inode_list[0].FT = DIRECTORY;
  inode_list[0].direct_blocks[0] = calloc(64, sizeof(char) * 16);

  super.root_inode = &inode_list[0];
  super.block_bitmap_offset = sizeof(super);
  super.block_bitmap_size = (sizeof(uint16_t) * (DISK_BLOCKS / 16));
  super.inode_bitmap_offset = super.block_bitmap_offset + super.block_bitmap_size;
  super.inode_bitmap_size = (sizeof(uint16_t) * (INODE_NUMBER / 16));
  super.inode_list_offset = super.inode_bitmap_offset + super.inode_bitmap_size;
  super.inode_list_size = sizeof(struct inode) * INODE_NUMBER;
  super.data_blocks_offset = super.inode_list_size + super.inode_list_offset;
  super.used_blocks_count = (super.data_blocks_offset + BLOCK_SIZE - 1) / BLOCK_SIZE;
  
  for(int i = 0; i < super.used_blocks_count; i++) {
    block_bitmap[i] = modifyBit(block_bitmap[i], i, 1);
  }

  char * buf = calloc(super.data_blocks_offset, sizeof(char));
  memcpy(buf, &super, super.block_bitmap_offset);
  memcpy(buf + (super.block_bitmap_offset), &block_bitmap, super.block_bitmap_size);
  memcpy(buf + (super.inode_bitmap_offset), &inode_bitmap, super.inode_bitmap_size);
  memcpy(buf + (super.inode_list_offset), &inode_list, super.inode_list_size);
  int length = super.data_blocks_offset;

  long unsigned * tmp_buf = calloc(BLOCK_SIZE, sizeof(char));
  
  int j = 0;
  
  while(length >= BLOCK_SIZE) {
    memcpy(tmp_buf, buf + (j * BLOCK_SIZE), BLOCK_SIZE);
    block_write(j,tmp_buf);
    j++;
    length = length - BLOCK_SIZE;
  }


  memcpy(tmp_buf, buf + (j * BLOCK_SIZE), length);
  block_write(j,tmp_buf);
  free(buf);
  free(tmp_buf);
  close_disk(disk_name);



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