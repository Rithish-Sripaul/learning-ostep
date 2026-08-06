#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"

int main(int argc, char* argv[]) {
  int* p = malloc(sizeof(int));
  assert(p != NULL);
  printf("(%d) address pointer to by p: %p\n", getpid(), p);
  // pid stands for process id

  *p = 0;
  while (1) {
    Spin(1);
    *p += 1;
    printf("(%d) p: %d\n", getpid(), *p);
  }
  return 0;
}