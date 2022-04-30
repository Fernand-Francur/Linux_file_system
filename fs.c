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
  
  char name[STRING_LEN]; 
  bool is_used;
  int inode_number;
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
  unsigned int direct_blocks[BLOCK_NUM];
  unsigned int indirect_blocks[BLOCK_NUM];
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

int find_free_bit(uint16_t n) {
  int free_bit = 0;
  for (int i = 1; i < 17; i++) {
    if (bit_ext(n, 1, i+1) == 0) {
      free_bit = i;
      break;
    }
  }
  return free_bit;
}

static unsigned int fd_num_used;
static uint16_t block_bitmap[DISK_BLOCKS / 16];
static uint16_t inode_bitmap[INODE_NUMBER / 16];
static fd fd_list[FILE_DESCRIPTOR];
static struct inode inode_list[INODE_NUMBER];
static god super;
static bool mounted = false;
static dir entries[FILE_NUM];

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
  inode_bitmap[0] = modifyBit(inode_bitmap[0], 0, 1);



  for (int i = 0; i < INODE_NUMBER; i++) {
    inode_list[i].FT = UNDEFINED;
    inode_list[i].ref_count = 0;
    inode_list[i].offset = 0;
    inode_list[i].file_size = 0;
  }

  for (int i = 0; i < FILE_NUM; i++) {
    entries[i].is_used = false;
    entries[i].inode_number = 0;
    strncpy(entries[i].name, "\0", STRING_LEN);
  }

  inode_list[0].FT = DIRECTORY;
  inode_list[0].ref_count = 0;
  inode_list[0].direct_blocks[0] = 2;
  inode_list[0].file_size = (64 * sizeof(dir));
  inode_list[0].offset = 0;

  super.root_inode = &inode_list[0];
  super.block_bitmap_offset = sizeof(super);
  super.block_bitmap_size = (sizeof(uint16_t) * (DISK_BLOCKS / 16));
  super.inode_bitmap_offset = super.block_bitmap_offset + super.block_bitmap_size;
  super.inode_bitmap_size = (sizeof(uint16_t) * (INODE_NUMBER / 16));
  super.inode_list_offset = super.inode_bitmap_offset + super.inode_bitmap_size;
  super.inode_list_size = sizeof(struct inode) * INODE_NUMBER;
  super.data_blocks_offset = super.inode_list_size + super.inode_list_offset;
  super.used_blocks_count = (super.data_blocks_offset + BLOCK_SIZE - 1) / BLOCK_SIZE + 1;
  //for(int i = 0; i <= super.used_blocks_count; i++) {
  block_bitmap[0] = modifyBit(block_bitmap[0], 0, 1);
  block_bitmap[0] = modifyBit(block_bitmap[0], 1, 1);
  block_bitmap[0] = modifyBit(block_bitmap[0], 2, 1);
  block_bitmap[0] = modifyBit(block_bitmap[0], 3, 1);
  block_bitmap[0] = modifyBit(block_bitmap[0], 4, 1);


  char * buf = calloc(super.data_blocks_offset, sizeof(char));
  memcpy(buf, &super, super.block_bitmap_offset);
  memcpy(buf + (super.block_bitmap_offset), &block_bitmap, super.block_bitmap_size);
  memcpy(buf + (super.inode_bitmap_offset), &inode_bitmap, super.inode_bitmap_size);
  memcpy(buf + (super.inode_list_offset), &inode_list, super.inode_list_size);
  int length = super.data_blocks_offset;

  char * tmp_buf = calloc(BLOCK_SIZE, sizeof(char));
  
  int j = 0;
  
  while(length >= BLOCK_SIZE) {
    memcpy(tmp_buf, buf + (j * BLOCK_SIZE), BLOCK_SIZE);
    block_write(j,tmp_buf);
    j++;
    length = length - BLOCK_SIZE;
  }


  memcpy(tmp_buf, buf + (j * BLOCK_SIZE), length);
  block_write(j,tmp_buf);
  j++;

  char * tmp_buf2 = calloc(BLOCK_SIZE, sizeof(char));
  memcpy(tmp_buf2, &entries, sizeof(entries));
  
  block_write(j, tmp_buf2);

  free(buf);
  free(tmp_buf);
  free(tmp_buf2);
  close_disk(disk_name);
  return 0;
}
int mount_fs(const char *disk_name) {

  if(mounted) {
    perror("ERROR: File system is already mounted");
    return -1;
  }

  if (open_disk(disk_name) == -1) {
    perror("ERROR: Invalid Disk name or disk is already open");
    return -1;
  }

  god new_sup;

  char * tmp_buf = calloc(BLOCK_SIZE, sizeof(char));

  block_read(0, tmp_buf);
  memcpy(&new_sup, tmp_buf, sizeof(new_sup));

  //printf("\nsize of block_bitmap_offset = %d\n", new_sup.block_bitmap_offset );
  //printf("size of block_bitmap_size = %d\n", new_sup.block_bitmap_size );
  //printf("size of inode bitmap = %d\n", new_sup.inode_bitmap_size );
  //printf("size of inode_bitmap_offset = %d\n", new_sup.inode_bitmap_offset );
  //printf("size of inode_list_offset = %d\n", new_sup.inode_list_offset );
  //printf("size of data_blocks_offset = %d\n", new_sup.data_blocks_offset );

  if ((new_sup.block_bitmap_offset != sizeof(new_sup)) ) {
    perror("ERROR: Invalid super block");
    return -1;
  }



  char * buf = calloc(new_sup.data_blocks_offset, sizeof(char));

  int length = super.data_blocks_offset;
  int j = 0;
  
  while(length >= BLOCK_SIZE) {
    block_read(j,tmp_buf);
    memcpy(buf + (j * BLOCK_SIZE), tmp_buf, BLOCK_SIZE);
    j++;
    length = length - BLOCK_SIZE;
  }

  block_read(j,tmp_buf);
  memcpy(buf + (j * BLOCK_SIZE), tmp_buf, length);

  memcpy(&super, buf, new_sup.block_bitmap_offset);
  memcpy(&block_bitmap, buf + (super.block_bitmap_offset) , super.block_bitmap_size);
  memcpy(&inode_bitmap, buf + (super.inode_bitmap_offset), super.inode_bitmap_size);
  memcpy(&inode_list, buf + (super.inode_list_offset), super.inode_list_size);  

  for (int i = 0; i < FILE_DESCRIPTOR; i++) {
    fd_list[i].is_used = false;
    fd_list[i].inode_num = -1;
    fd_list[i].offset = 0;
  }
  char * tmp_buf2 = calloc(BLOCK_SIZE, sizeof(char));
  block_read(3, tmp_buf2);

  memcpy(&entries, tmp_buf2, sizeof(entries));

  free(tmp_buf);
  free(tmp_buf2);
  free(buf);
  mounted = true;
  return 0;
}
int umount_fs(const char *disk_name) {

  if(!mounted) {
    perror("ERROR: File system is not mounted. Cannot umount");
    return -1;
  }
  char * tmp_buf = calloc(BLOCK_SIZE, sizeof(char));
  if(block_read(0,tmp_buf) == -1) {
    perror("ERROR: Disk is not open");
    return -1;
  }
  god new_sup;

  memcpy(&new_sup, tmp_buf, sizeof(new_sup));
  if ((new_sup.block_bitmap_offset != sizeof(new_sup)) ) {
    perror("ERROR: Invalid super block");
    return -1;
  }

  char * buf = calloc(super.data_blocks_offset, sizeof(char));
  memcpy(buf, &super, super.block_bitmap_offset);
  memcpy(buf + (super.block_bitmap_offset), &block_bitmap, super.block_bitmap_size);
  memcpy(buf + (super.inode_bitmap_offset), &inode_bitmap, super.inode_bitmap_size);
  memcpy(buf + (super.inode_list_offset), &inode_list, super.inode_list_size);
  int length = super.data_blocks_offset;
  int j = 0;
  
  while(length >= BLOCK_SIZE) {
    memcpy(tmp_buf, buf + (j * BLOCK_SIZE), BLOCK_SIZE);
    block_write(j,tmp_buf);
    j++;
    length = length - BLOCK_SIZE;
  }


  memcpy(tmp_buf, buf + (j * BLOCK_SIZE), length);
  block_write(j,tmp_buf);
  j++;
  char * tmp_buf2 = calloc(BLOCK_SIZE, sizeof(char));
  memcpy(tmp_buf2, &entries, sizeof(entries));
  block_write(j, tmp_buf2);
  
  free(buf);
  free(tmp_buf);
  free(tmp_buf2);
  close_disk(disk_name);
  mounted = false;

  return 0;
}
int fs_open(const char *name) {

  int entry = 1000;
  
  for (int i = 0; i < FILE_NUM; i++) {
    if (entries[i].is_used == true) {
      if(strcmp(entries[i].name, name) == 0) {
	entry = i;
	break;
      }
    }
  }
  if (entry == 1000) {
    perror("ERROR: No such file exists with this name");
    return -1;
  }

  int unused_fd = 33;
  for (int j = 0; j < FILE_DESCRIPTOR; j++) {
    if(fd_list[j].is_used == false) {
      unused_fd = j;
      break;
    }
  }
  if (unused_fd == 33) {
    perror("ERROR: All file file_descripters are busy. Close a file");
    return -1;
  }

  
  fd_list[unused_fd].is_used = true;
  fd_list[unused_fd].inode_num = entries[entry].inode_number;
  fd_list[unused_fd].offset = 0;

  inode_list[entries[entry].inode_number].ref_count++;

  return unused_fd;
} 

int fs_close(int fildes) {
  if ((fildes < 0) || (fildes > 31)) {
    perror("ERROR: File descriptor out of range");
    return -1;
  }
  if (fd_list[fildes].is_used == false) {
    perror("ERROR: File descriptor is not open");
    return -1;
  }
  fd_list[fildes].offset = 0;

  inode_list[fd_list[fildes].inode_num].ref_count--;
  fd_list[fildes].inode_num = -1;
  
  fd_list[fildes].is_used = false;


  return 0;
}




int fs_create(const char *name) {
  if(strlen(name) > 15) {
    perror("ERROR: Name size out of bounds");
    return -1;
  }
  int unused = 65;

  
  for (int i = 0; i < FILE_NUM; i++) {
    if (entries[i].is_used == true) {
      if(strcmp(entries[i].name, name) == 0) {
        perror("ERROR: Filename already exists");
        return -1;
      }
    } else {
      unused = i;
      break;
    }
  }
  if (unused == 65) {
    perror("ERROR: Too many files created. Delete a file to make a new one");
    return -1;
  }
  int unused_inode_in_index = 65;
  int inode_index = 0;
  for (int i = 0; i < (INODE_NUMBER / 16); i++) {
    for(int j = 1; j < 17; j++) {
      if (bit_ext(inode_bitmap[i],1,j) == 0) {
        unused_inode_in_index = j;
        inode_index = i;
        break;
      }
    }
    if (unused_inode_in_index != 65) {
        break;
    }
  }
  if (unused_inode_in_index == 65) {
    perror("ERROR: No available inodes. Delete a file");
    return -1;
  }

  strncpy(entries[unused].name,name, strlen(name) + 1);;
  entries[unused].is_used = true;
  entries[unused].inode_number = inode_index * 16 + unused_inode_in_index;
  inode_bitmap[inode_index] = modifyBit(inode_bitmap[inode_index], unused_inode_in_index - 1, 1);
  inode_list[entries[unused].inode_number].FT = FILE_TYPE;

  return 0;
}
int fs_delete(const char *name) {

  int entry = 1000;

  for (int i = 0; i < FILE_NUM; i++) {
    if (entries[i].is_used == true) {
      if(strcmp(entries[i].name, name) == 0) {
	entry = i;
	break;
      }
    }
  }
  if (entry == 1000) {
    perror("ERROR: No such file exists with this name");
    return -1;
  }

  int inode_num = entries[entry].inode_number;
  if (inode_list[inode_num].ref_count != 0) {
    perror("ERROR: Cannot delete, file is open");
    return -1;
  }

  for(int i = 0; i < FILE_DESCRIPTOR; i++) {
    if(fd_list[i].is_used == true) {
      if(fd_list[i].inode_num == inode_num) {
	perror("ERROR: Cannot delete, file is open");
	return -1;
      }
    }
  }

  char * clean = calloc(BLOCK_SIZE, sizeof(char));
  
  for(int i = 0; i < BLOCK_NUM; i++) {
    if (inode_list[inode_num].indirect_blocks[i] != 0) {
      char * tmp_buf = calloc(BLOCK_SIZE, sizeof(char));
      int ind_block[BLOCK_SIZE / sizeof(int)];
      block_read(inode_list[inode_num].indirect_blocks[i], tmp_buf);
      memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
      for( int j = 0; j < (BLOCK_SIZE / sizeof(int)); j++) {
	if(ind_block[j] != 0) {
	  block_write(j, clean);
	  int bitmap_index = ind_block[j] / 16;
	  int bit = ind_block[j] - 16 * bitmap_index;
	  block_bitmap[bitmap_index] = modifyBit(block_bitmap[bitmap_index], bit, 0);
	}
      }
      free(tmp_buf);
      block_write(inode_list[inode_num].indirect_blocks[i], clean);
      inode_list[inode_num].indirect_blocks[i] = 0;
    }
  }

  for(int i = 0; i < BLOCK_NUM; i++) {
    if (inode_list[inode_num].direct_blocks[i] != 0) {
      block_write(i, clean);
      int bitmap_index = inode_list[inode_num].direct_blocks[i] / 16;
      int bit = inode_list[inode_num].direct_blocks[i] - 16 * bitmap_index;
      block_bitmap[bitmap_index] = modifyBit(block_bitmap[bitmap_index], bit, 0);
      inode_list[inode_num].direct_blocks[i] = 0;
    }
  }

  inode_list[inode_num].FT = UNDEFINED;
  inode_list[inode_num].ref_count = 0;
  inode_list[inode_num].offset = 0;
  inode_list[inode_num].file_size = 0;
  int inode_index = inode_num / 16;
  int bit = inode_num - 16 * inode_index;
  inode_bitmap[inode_index] = modifyBit(inode_bitmap[inode_index], bit, 0);

  strncpy(entries[entry].name, "\0", STRING_LEN);
  entries[entry].is_used = false;
  entries[entry].inode_number = 0;
  
  free(clean);
  return 0;
}


int fs_read(int fildes, void *buf, size_t nbyte) {
  if ((fildes < 0) || (fildes > 31)) {
    perror("ERROR: File descriptor out of range");
    return -1;
  }
  if (fd_list[fildes].is_used == false) {
    perror("ERROR: File descriptor is not open");
    return -1;
  }

  return 0;
}





int fs_write(int fildes, void *buf, size_t nbyte) {
  if ((fildes < 0) || (fildes > 31)) {
    perror("ERROR: File descriptor out of range");
    return -1;
  }
  if (fd_list[fildes].is_used == false) {
    perror("ERROR: File descriptor is not open");
    return -1;
  }
  char * ari_buf = (char *) buf;
  int inode_num = fd_list[fildes].inode_num;
  int current_block = inode_list[inode_num].offset;
  
  int length = nbyte;
  
  char * tmp_buf = calloc(BLOCK_SIZE, sizeof(char));

  
  int offset_in_current_block = fd_list[fildes].offset;

  while (offset_in_current_block > 4095) {
    offset_in_current_block = offset_in_current_block - 4096;
  }

  int space_in_current_block = BLOCK_SIZE - offset_in_current_block;
  int current_indirect = 0;
  bool start_in_indirect = false;
  int ind_block[BLOCK_SIZE / sizeof(int)];
  int original_block_start = current_block;
  char * tmp_buf2 = calloc(BLOCK_SIZE, sizeof(char));
  
  if (current_block > 9) {
    current_block = current_block - 10;
    start_in_indirect = true;
    while (current_block > 1023) {
      current_block = current_block - 1024;
      current_indirect++;
    }
  }
  if (length > space_in_current_block) {
    memcpy(tmp_buf2 + offset_in_current_block, buf, space_in_current_block);
  
    if (start_in_indirect) {
      block_read(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
      memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
      block_write(ind_block[current_block], tmp_buf2);
    } else {
      block_write(inode_list[inode_num].direct_blocks[original_block_start], tmp_buf2);
    }
    
    length = length - space_in_current_block;
    int length_copied = space_in_current_block;
    current_block = current_block + 1;
    
    while (length > BLOCK_SIZE) {
      int before_mod = current_block;
      if ((current_block > 9) && (!start_in_indirect) ) {
	current_block = current_block - 10;
	start_in_indirect = true;
	if (inode_list[inode_num].indirect_blocks[current_indirect] == 0) {
	  bool free_bit_found = false;
	  for (int j = 0; j < (DISK_BLOCKS / 16); j++) {
	    int internal_bit = find_free_bit(block_bitmap[j]);
	    if (internal_bit != 0) {
	      internal_bit--;
	      block_bitmap[j] = modifyBit(block_bitmap[j], internal_bit, 1);
	      inode_list[inode_num].indirect_blocks[current_indirect] = j*16 + internal_bit;
	      free_bit_found = true;
	      break;
	    }
	  }
	  if (free_bit_found == false) {
	    printf("No free bit found");
	    fd_list[fildes].offset = fd_list[fildes].offset + nbyte - length;
	    inode_list[inode_num].offset = fd_list[fildes].offset / BLOCK_SIZE;
	    inode_list[inode_num].file_size = inode_list[inode_num].file_size + nbyte - length;
	    return nbyte - length;
	  }
	  free_bit_found = false;
	  for (int j = 0; j < (DISK_BLOCKS / 16); j++) {
	    int internal_bit = find_free_bit(block_bitmap[j]);
	    if (internal_bit != 0) {
	      internal_bit--;
	      block_bitmap[j] = modifyBit(block_bitmap[j], internal_bit, 1);
	      block_read(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	      memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
	      ind_block[0] = j*16 + internal_bit;
	      memcpy( tmp_buf, &ind_block, BLOCK_SIZE);
	      block_write(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	      free_bit_found = true;
	      break;
	    }
	  }
	  if (free_bit_found == false) {
	    printf("No free bit found");
	    fd_list[fildes].offset = fd_list[fildes].offset + nbyte - length;
	    inode_list[inode_num].offset = fd_list[fildes].offset / BLOCK_SIZE;
	    inode_list[inode_num].file_size = inode_list[inode_num].file_size + nbyte - length;
	    return nbyte - length;
	  }
	}
      } else if ((current_block > 1023) && (start_in_indirect)) {
	current_block = current_block - 1024;
	current_indirect++;

	if (inode_list[inode_num].indirect_blocks[current_indirect] == 0) {
	  bool free_bit_found = false;
	  for (int j = 0; j < (DISK_BLOCKS / 16); j++) {
	    int internal_bit = find_free_bit(block_bitmap[j]);
	    if (internal_bit != 0) {
	      internal_bit--;
	      block_bitmap[j] = modifyBit(block_bitmap[j], internal_bit, 1);
	      inode_list[inode_num].indirect_blocks[current_indirect] = j*16 + internal_bit;
	      free_bit_found = true;
	      break;
	    }
	  }
	  if (free_bit_found == false) {
	    printf("No free bit found");
	    fd_list[fildes].offset = fd_list[fildes].offset + nbyte - length;
	    inode_list[inode_num].offset = fd_list[fildes].offset / BLOCK_SIZE;
	    inode_list[inode_num].file_size = inode_list[inode_num].file_size + nbyte - length;
	    return nbyte - length;
	  }
	  free_bit_found = false;
	  for (int j = 0; j < (DISK_BLOCKS / 16); j++) {
	    int internal_bit = find_free_bit(block_bitmap[j]);
	    if (internal_bit != 0) {
	      internal_bit--;
	      block_bitmap[j] = modifyBit(block_bitmap[j], internal_bit, 1);
	      block_read(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	      memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
	      ind_block[0] = j*16 + internal_bit;
	      memcpy( tmp_buf, &ind_block, BLOCK_SIZE);
	      block_write(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	      free_bit_found = true;
	      break;
	    }
	  }
	}
      }
      if (current_block != before_mod) {
	block_read(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
	memcpy(tmp_buf2, ari_buf + length_copied, BLOCK_SIZE);
	block_write(ind_block[current_block], tmp_buf2);
      } else {
	memcpy(tmp_buf2, ari_buf + length_copied, BLOCK_SIZE);
	if (start_in_indirect) {
	  block_read(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	  memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
	  block_write(ind_block[current_block], tmp_buf2);
	} else {
	  block_write(inode_list[inode_num].direct_blocks[current_block], tmp_buf2);
	}
      }
	
      length_copied = length_copied + BLOCK_SIZE;
      current_block++;
      length = length - BLOCK_SIZE;
    }



    int before_mod = current_block;
    if ((current_block > 9) && (!start_in_indirect) ) {
      current_block = current_block - 10;
      start_in_indirect = true;
      if (inode_list[inode_num].indirect_blocks[current_indirect] == 0) {
	bool free_bit_found = false;
	for (int j = 0; j < (DISK_BLOCKS / 16); j++) {
	  int internal_bit = find_free_bit(block_bitmap[j]);
	  if (internal_bit != 0) {
	    internal_bit--;
	    block_bitmap[j] = modifyBit(block_bitmap[j], internal_bit, 1);
	    inode_list[inode_num].indirect_blocks[current_indirect] = j*16 + internal_bit;
	    free_bit_found = true;
	    break;
	  }
	}
	if (free_bit_found == false) {
	  printf("No free bit found");
	  fd_list[fildes].offset = fd_list[fildes].offset + nbyte - length;
	  inode_list[inode_num].offset = fd_list[fildes].offset / BLOCK_SIZE;
	  inode_list[inode_num].file_size = inode_list[inode_num].file_size + nbyte - length;
	  return nbyte - length;
	}
	free_bit_found = false;
	for (int j = 0; j < (DISK_BLOCKS / 16); j++) {
	  int internal_bit = find_free_bit(block_bitmap[j]);
	  if (internal_bit != 0) {
	    internal_bit--;
	    block_bitmap[j] = modifyBit(block_bitmap[j], internal_bit, 1);
	    block_read(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	    memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
	    ind_block[0] = j*16 + internal_bit;
	    memcpy( tmp_buf, &ind_block, BLOCK_SIZE);
	    block_write(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	    free_bit_found = true;
	    break;
	  }
	}
	if (free_bit_found == false) {
	  printf("No free bit found");
	  fd_list[fildes].offset = fd_list[fildes].offset + nbyte - length;
	  inode_list[inode_num].offset = fd_list[fildes].offset / BLOCK_SIZE;
	  inode_list[inode_num].file_size = inode_list[inode_num].file_size + nbyte - length;
	  return nbyte - length;
	}
      }
    } else if ((current_block > 1023) && (start_in_indirect)) {
      current_block = current_block - 1024;
      current_indirect++;

      if (inode_list[inode_num].indirect_blocks[current_indirect] == 0) {
	bool free_bit_found = false;
	for (int j = 0; j < (DISK_BLOCKS / 16); j++) {
	  int internal_bit = find_free_bit(block_bitmap[j]);
	  if (internal_bit != 0) {
	    internal_bit--;
	    block_bitmap[j] = modifyBit(block_bitmap[j], internal_bit, 1);
	    inode_list[inode_num].indirect_blocks[current_indirect] = j*16 + internal_bit;
	    free_bit_found = true;
	    break;
	  }
	}
	if (free_bit_found == false) {
	  printf("No free bit found");
	  fd_list[fildes].offset = fd_list[fildes].offset + nbyte - length;
	  inode_list[inode_num].offset = fd_list[fildes].offset / BLOCK_SIZE;
	  inode_list[inode_num].file_size = inode_list[inode_num].file_size + nbyte - length;
	  return nbyte - length;
	}
	free_bit_found = false;
	for (int j = 0; j < (DISK_BLOCKS / 16); j++) {
	  int internal_bit = find_free_bit(block_bitmap[j]);
	  if (internal_bit != 0) {
	    internal_bit--;
	    block_bitmap[j] = modifyBit(block_bitmap[j], internal_bit, 1);
	    block_read(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	    memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
	    ind_block[0] = j*16 + internal_bit;
	    memcpy( tmp_buf, &ind_block, BLOCK_SIZE);
	    block_write(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
	    free_bit_found = true;
	    break;
	  }
	}
      }
    }
    if (current_block != before_mod) {
      block_read(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
      memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
      memcpy(tmp_buf2, ari_buf + length_copied, length);
      block_write(ind_block[current_block], tmp_buf2);
    } else {
      memcpy(tmp_buf2, ari_buf + length_copied, length);
      block_write(inode_list[inode_num].direct_blocks[current_block], tmp_buf2);
    }
    length_copied = length_copied + length;
    length = 0;
  } else {
    memcpy(tmp_buf2 + offset_in_current_block, buf, length);
    if (start_in_indirect) {
      block_read(inode_list[inode_num].indirect_blocks[current_indirect], tmp_buf);
      memcpy(&ind_block, tmp_buf, BLOCK_SIZE);
      block_write(ind_block[current_block], tmp_buf2);
    } else {
      block_write(inode_list[inode_num].direct_blocks[original_block_start], tmp_buf2);
    }
    length = 0;
  }
  
  
  
  free(tmp_buf2);
  free(tmp_buf);

  fd_list[fildes].offset = fd_list[fildes].offset + nbyte - length;
  inode_list[inode_num].offset = fd_list[fildes].offset / BLOCK_SIZE;
  inode_list[inode_num].file_size = inode_list[inode_num].file_size + nbyte - length;
  return nbyte - length;
}


int fs_get_filesize(int fildes) {
  if ((fildes < 0) || (fildes > 31)) {
    perror("ERROR: File descriptor out of range");
    return -1;
  }
  if (fd_list[fildes].is_used == false) {
    perror("ERROR: File descriptor is not open");
    return -1;
  }
  
  return inode_list[fd_list[fildes].inode_num].file_size;
}


int fs_listfiles(char ***files) {
  return 0;
}
int fs_lseek(int fildes, off_t offset) {
  if ((fildes < 0) || (fildes > 31)) {
    perror("ERROR: File descriptor out of range");
    return -1;
  }
  if (fd_list[fildes].is_used == false) {
    perror("ERROR: File descriptor is not open");
    return -1;
  }
  int size_of_file = inode_list[fd_list[fildes].inode_num].file_size;
  if ((offset < 0) || (offset > size_of_file)) {
    perror("ERROR: lseek offsets outside of parameters of file");
    return -1;
  }

  fd_list[fildes].offset = offset;
  inode_list[fd_list[fildes].inode_num].offset = offset / BLOCK_SIZE;

  return 0;
}
int fs_truncate(int fildes, off_t length) {
  if ((fildes < 0) || (fildes > 31)) {
    perror("ERROR: File descriptor out of range");
    return -1;
  }
  if (fd_list[fildes].is_used == false) {
    perror("ERROR: File descriptor is not open");
    return -1;
  }

  return 0;
}
