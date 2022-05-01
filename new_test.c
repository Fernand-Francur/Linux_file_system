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
    char name[10] = "working";
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
    int fd = fs_open(file_name2);

    char latin[1001] = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. N";
    char * copy_write = calloc(1000, sizeof(char));
    memcpy(copy_write, &latin, 1000);
    fs_write(fd, copy_write, 1000);

    char * copy_read = calloc(1000, sizeof(char));
    fs_lseek(fd, 500);

    char * two_write = calloc(2, sizeof(char));
    char two[3] = "99";
    memcpy(two_write, &two, 2);
    fs_write(fd, two_write, 2);

    fs_lseek(fd, 0);
    fs_read(fd, copy_read, 1000);
    char latin_copy[1001];
    memcpy(&latin_copy, copy_read, 1000);

    //printf("%s\n", latin_copy);


    fs_close(fd);
    umount_fs(name);

    mount_fs(name);
    umount_fs(name);
    
    free(two_write);
    free(copy_write);
    free(copy_read);
    printf("end\n");
  return 0;
}
