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
    char name[10] = "godwhy7m";
    char file_name1[16] = "newfile";
    // close_disk(name);
    // umount_fs(name);
    printf("start\n");


    make_fs(name);
    mount_fs(name);
    fs_create(file_name1);
    umount_fs(name);


    printf("end\n");
  return 0;
}