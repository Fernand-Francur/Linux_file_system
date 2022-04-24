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
    char name[10] = "cursed";
    char file_name1[16] = "fudgefile";
    char file_name2[16] = "dyinginside";
    char file_name3[16] = "crying";
    char file_name4[16] = "suffering";
    char file_name5[16] = "askingwhy";
    char file_name6[16] = "thiscode";
    char file_name7[16] = "doesntpass";
    char file_name8[16] = "case6.2";
    char file_name9[16] = "sadness";
    char file_name10[16] = "depression";
    char file_name11[16] = "justice";
    char file_name12[16] = "incineration";
    char file_name13[16] = "burnbright";
    char file_name14[16] = "flightpath";
    char file_name15[16] = "mylife";
    char file_name16[16] = "OPERATING";
    //close_disk(name);
    //umount_fs(name);
    printf("start\n");


    make_fs(name);
    mount_fs(name);



    fs_create(file_name1);
    fs_create(file_name2);
    fs_create(file_name3);
    fs_create(file_name4);
    fs_create(file_name5);
    fs_create(file_name6);
    fs_create(file_name7);
    fs_create(file_name8);
    fs_create(file_name9);
    fs_create(file_name10);
    fs_create(file_name11);
    fs_create(file_name12);
    fs_create(file_name13);
    fs_create(file_name14);
    fs_create(file_name15);
    fs_create(file_name16);
    
    fs_create(file_name1);
    umount_fs(name);


    printf("end\n");
  return 0;
}
