#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "Fork Failed.\n");
    exit(1);
  } else if (rc == 0) {
    printf("Closing STDOUT\n");
    close(STDOUT_FILENO);
    printf("Can we still see this?\n");
  } else {
    int rc_wait = wait(NULL);
    printf("Something\n");
  }
  return 0;
}