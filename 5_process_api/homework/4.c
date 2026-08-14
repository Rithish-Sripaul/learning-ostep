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
    char* args[3];
    args[0] = strdup("/bin/ls");
    args[1] = strdup(argv[1]);
    args[2] = NULL;

    execl(args[0], args[1], NULL);
    // execvp(args[0], args);
    // execlp(args[0], args[1], NULL);
  } else {
    int rc_wait = wait(NULL);
    printf("Program end.\n");
  }

  return 0;
}