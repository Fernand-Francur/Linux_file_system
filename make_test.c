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
    char name[10] = "godwhy1";
    printf("start\n");
    make_fs(name);
    mount_fs(name);
    umount_fs(name);
    printf("end\n");
  return 0;
}