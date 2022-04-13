#include <stdlib.h>
#include <stdbool.h>
#include <setjmp.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

int make_fs(const char *disk_name) {
  return 0;
}
int mount_fs(const char *disk_name) {
  return 0;
}
int umount_fs(const char *disk_name) {
  return 0;
}
int fs_open(const char *name) {
  return 0;
}
int fs_close(int fildes) {
  return 0;
}
int fs_create(const char *name) {
  return 0;
}
int fs_delete(const char *name) {
  return 0;
}
int fs_read(int fildes, void *buf, size_t nbyte) {
  return 0;
}
int fs_write(int fildes, void *buf, size_t nbyte) {
  return 0;
}
int fs_get_filesize(int fildes) {
  return 0;
}
int fs_listfiles(char ***files) {
  return 0;
}
int fs_lseek(int fildes, off_t offset) {
  return 0;
}
int fs_truncate(int fildes, off_t length) {
  return 0;
}