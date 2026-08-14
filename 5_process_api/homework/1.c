#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char* argv[]) {
  int x = 2;

  printf("parent x: %d (pid: %d)\n", x, (int)getpid());
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork failed.\n");
    exit(1);
  } else if (rc == 0) {
    printf("child x: %d (pid: %d)\n", x, (int)getpid());
    x = 4;
  } else {
    int x = 4;
    int rc_wait = wait(NULL);
    printf("parent x: %d (pid: %d)\n", x, (int)getpid());
  }
  return 0;
}