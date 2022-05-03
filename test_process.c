#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "fs.c"
#include "disk.h"

int main() {
  char name[10] = "Megabyteh";

  char filename_meg[16] = "Mega";

  char * read_40 = calloc(1024 * 1024, sizeof(char));
  
  mount_fs(name);

  int fd_meg = fs_open(filename_meg);
  
  int how_much = fs_read(fd_meg, read_40, 1024 * 1024);
  printf("%d\n" , how_much);
  
  umount_fs(name);

  free(read_40);
  return 0;
}
