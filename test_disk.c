#include <stdlib.h>
#include <stdbool.h>
#include <setjmp.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include "disk.h"

int main() {
  char name[10] = "testing";
  char read[BLOCK_SIZE];
  open_disk(name);
  block_write(0, name);
  block_read(0, read);
  printf("Read says %s\n", read);
  close_disk(name);
  return 0;
}