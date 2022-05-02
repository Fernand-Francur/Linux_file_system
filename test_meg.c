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
  char name[10] = "Megabyte9";
  char filename_meg[16] = "Mega";
  char place_holder_file[16] = "Other";

  char random[100] = "Far far away, behind the word mountains, far from the countries Vokalia and Consonantia, there live";
  
  char latin[1025] = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit ve";

  char * extra = calloc(99, sizeof(char));
  char * meg = calloc(1024, sizeof(char));
  memcpy(extra, &random, 99);
  memcpy(meg, &latin, 1024);
  
  make_fs(name);
  mount_fs(name);

  fs_create(filename_meg);
  fs_create(place_holder_file);

  int fd_meg = fs_open(filename_meg);
  int fd_other = fs_open(place_holder_file);

  fs_write(fd_other, extra, 99);
  
  for (int i = 0; i < (1024 * 40); i++) {
    fs_write(fd_meg, meg, 1024);
  }

  char * read_40 = calloc(1024 * 1024 * 40, sizeof(char));

  fs_lseek(fd_meg, 0);
  
  int how_much = fs_read(fd_meg, read_40, 1024 * 1024 * 40);
  printf("%d\n" , how_much);
  fs_close(fd_meg);
  fs_close(fd_other);

  umount_fs(name);
  free(read_40);
  free(meg);
  free(extra);

  
  return 0;
}
