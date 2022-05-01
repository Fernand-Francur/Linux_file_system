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
    char name[10] = "corr";
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
    char file_name17[16] = "a";
    char file_name18[16] = "b";
    char file_name19[16] = "c";
    char file_name20[16] = "d";
    char file_name21[16] = "e";
    char file_name22[16] = "f";
    char file_name23[16] = "g";
    char file_name24[16] = "h";
    char file_name25[16] = "i";
    char file_name26[16] = "j";
    char file_name27[16] = "k";
    char file_name28[16] = "l";
    char file_name29[16] = "m";
    char file_name30[16] = "n";
    char file_name31[16] = "o";
    char file_name32[16] = "P";
    char file_name33[16] = "q";
    char file_name34[16] = "rdyinginside";
    char file_name35[16] = "scrying";
    char file_name36[16] = "tsuffering";
    char file_name37[16] = "uaskingwhy";
    char file_name38[16] = "vthiscode";
    char file_name39[16] = "wdoesntpass";
    char file_name40[16] = "xcase6.2";
    char file_name41[16] = "ysadness";
    char file_name42[16] = "zdepression";
    char file_name43[16] = "1justice";
    char file_name44[16] = "2incineration";
    char file_name45[16] = "3burnbright";
    char file_name46[16] = "4flightpath";
    char file_name47[16] = "5mylife";
    char file_name48[16] = "6OPERATING";
    char file_name49[16] = "7fudgefile";
    char file_name50[16] = "8dyinginside";
    char file_name51[16] = "9crying";
    char file_name52[16] = "10suffering";
    char file_name53[16] = "ww";
    char file_name54[16] = "tt";
    char file_name55[16] = "sdfa";
    char file_name56[16] = "das";
    char file_name57[16] = "wer";
    char file_name58[16] = "dtre";
    char file_name59[16] = "juce";
    char file_name60[16] = "incition";
    char file_name61[16] = "burght";
    char file_name62[16] = "fligth";
    char file_name63[16] = "myfe";
    char file_name64[16] = "OPING";

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
    fs_create(file_name17);
    fs_create(file_name18);
    fs_create(file_name19);
    fs_create(file_name20);
    fs_create(file_name21);
    fs_create(file_name22);
    fs_create(file_name23);
    fs_create(file_name24);
    fs_create(file_name25);
    fs_create(file_name26);
    fs_create(file_name27);
    fs_create(file_name28);
    fs_create(file_name29);
    fs_create(file_name30);
    fs_create(file_name31);
    fs_create(file_name32);
    fs_create(file_name33);
    fs_create(file_name34);
    fs_create(file_name35);
    fs_create(file_name36);
    fs_create(file_name37);
    fs_create(file_name38);
    fs_create(file_name39);
    fs_create(file_name40);
    fs_create(file_name41);
    fs_create(file_name42);
    fs_create(file_name43);
    fs_create(file_name44);
    fs_create(file_name45);
    fs_create(file_name46);
    fs_create(file_name47);
    fs_create(file_name48);
    fs_create(file_name49);
    fs_create(file_name50);
    fs_create(file_name51);
    fs_create(file_name52);
    fs_create(file_name53);
    fs_create(file_name54);
    fs_create(file_name55);
    fs_create(file_name56);
    fs_create(file_name57);
    fs_create(file_name58);
    fs_create(file_name59);
    fs_create(file_name60);
    fs_create(file_name61);
    fs_create(file_name62);
    fs_create(file_name63);
    fs_create(file_name64);
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

    printf("%s\n", latin_copy);
    char **files = calloc(64, sizeof(char *));
    for(int i = 0; i < 64; i++) {
      files[i] = calloc(16, sizeof(char));
    }
    fs_listfiles(&files);
    for(int i = 0; i < 64; i++) {
      printf("%s\n", files[i]);
      free(files[i]);
    }
    
    free(files);
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
