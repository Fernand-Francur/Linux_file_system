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

int bit_ext(uint64_t num, int k, int p) {
  return (((1 << k) - 1) & (num >> (p - 1)));
}

int modifyBit(int n, int p, int b)
{
    int mask = 1 << p;
    return ((n & ~mask) | (b << p));
}

int calc_off(unsigned int offset) {
  return offset / 64;
}

int main() {
  uint64_t bitmap[16];

  unsigned int offset = 0;
  unsigned int a = calc_off(offset);
  printf("Size of uint16_t = %ld\n", sizeof(bitmap[a])*8);
  bitmap[a] = 0;
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 1));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 2));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 3));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 4));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 5));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 6));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 7));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 8));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 9));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 10));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 11));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 12));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 13));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 14));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 15));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 16));



  bitmap[a] = modifyBit(bitmap[a], 0, 1);
  printf("Bit modified\n");

  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 1));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 2));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 3));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 4));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 5));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 6));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 7));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 8));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 9));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 10));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 11));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 12));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 13));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 14));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 15));
  printf("The extracted num is %d\n", bit_ext(bitmap[a], 1, 16));


  return 0;
}