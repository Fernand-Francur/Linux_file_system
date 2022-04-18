#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include "fs.c"
#include "disk.h"

int main() {

  //struct inode test_node;
  god super;
  super.used_blocks_count = 171;
  unsigned long * buf = calloc(1, 4096);
  char name[10] = "testing";

  open_disk(name);

  printf("Size of superblock %lu \n", sizeof(super));

  uint16_t block_bitmap[DISK_BLOCKS / 16];
  uint16_t inode_bitmap[INODE_NUMBER / 4];

  for (int i = 0; i < (512/*DISK_BLOCKS/16*/); i++) {
    for (int j = 0; j < 16; j++) {
      modifyBit(block_bitmap[i], j, 1);
      printf("%d\n",bit_ext(block_bitmap[i], 16, 1));

    }
    printf("\nWhat number: %d\n", block_bitmap[i]);
  }

  // for (int i = 0; i < (DISK_BLOCKS/16); i++) {
  //   block_bitmap[i] = -1;
  // }

  for (int i = 0; i < (INODE_NUMBER / 4); i++) {
    for (int j = 0; j < 16; j++) {
      modifyBit(inode_bitmap[i], j, 1);
    }
  }
  for (int i = 0; i < (INODE_NUMBER / 4); i++) {
    inode_bitmap[i] = 16;
  }

  printf("block_bitmap should be maxed = %d\n", block_bitmap[0]);

  super.block_bitmap_offset = sizeof(super);
  super.block_bitmap_size = (sizeof(uint16_t) * (DISK_BLOCKS / 16));
  super.inode_bitmap_size = (sizeof(uint16_t) * (INODE_NUMBER / 4));
  super.inode_bitmap_offset = super.block_bitmap_offset + super.block_bitmap_size;
  super.inode_list_offset = super.inode_bitmap_offset + super.inode_bitmap_size;

  printf("size of inode bitmap = %d\n", super.inode_bitmap_size );

  memcpy(buf, &super, super.block_bitmap_offset);
  memcpy(buf + super.block_bitmap_offset, &block_bitmap, super.block_bitmap_size);
  memcpy(buf + super.inode_bitmap_offset, &inode_bitmap, super.inode_bitmap_size);

  block_write(0, buf);

  god new_sup;

  unsigned long * new_buf = calloc(1,4096);
  block_read(0, new_buf);
  uint16_t block_bitmap_2[DISK_BLOCKS / 16];
  uint16_t inode_bitmap_2[INODE_NUMBER / 4];

  memcpy(&new_sup, new_buf, sizeof(new_sup));
  memcpy(&block_bitmap_2, new_buf + new_sup.block_bitmap_offset, new_sup.block_bitmap_size);
  memcpy(&inode_bitmap_2, new_buf + new_sup.inode_bitmap_offset, new_sup.inode_bitmap_size);

  printf("Int = %d\n offset = %d\n", new_sup.used_blocks_count, new_sup.block_bitmap_offset);
  for(int i = 0; i < new_sup.inode_bitmap_size; i++) {
    printf("%d\n", inode_bitmap_2[i]);
  }



  close_disk(name);

  return 0;
}