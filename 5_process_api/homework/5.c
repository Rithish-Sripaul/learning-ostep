#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed.\n");
    exit(1);
  } else if (rc == 0) {
    // printf("Child first. rc_wait: %d\n", rc_wait);
    printf("Child first.\n");
  } else {
        int rc_wait = waitpid(rc, NULL, 0);
    printf("Parent Next. rc_wait: %d\n", rc_wait);
  }

  return 0;
}