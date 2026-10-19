#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork failed.\n");
    exit(1);
  } else if (rc == 0) {
    printf("[CHILD: %d] Started doing some heavy work...\n", (int)getpid());
    sleep(3);

    printf("[CHILD: %d] Work finished. Exiting.\n", (int)getpid());
    exit(0);
  } else {
    int status;
    pid_t result;

    printf("[PARENT: %d] Monitoring child process (PID: %d)...", (int)getpid(),
           rc);

    while (1) {
      result = waitpid(rc, &status, WNOHANG);
      if (result == 0) {
        printf(
            "[PARENT: %d] Child is still busy. Parent is doing other work...",
            (int)getpid());
        sleep(1);
      } else if (result == rc) {
        printf("[PARENT: %d] CHILD has terminated. Parent not exiting. \n",
               (int)getpid());
        break;
      } else {
        perror("waitpid failed\n");
        exit(1);
        
      }
    }
  }

  return 0;
}