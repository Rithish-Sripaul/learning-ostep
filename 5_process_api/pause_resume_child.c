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
    printf("[CHILD] My PID is %d. I am running...\n", (int)getpid());

    while (1) {
      printf("[CHILD] Still working...\n");
      sleep(1);
    }
  } else {
    int status;
    printf("[PARENT] Monitoring child process %d.\n", rc);
    printf("\n---> Open another terminal and try these commands:\n");
    printf("\t kill -STOP %d (Pauses the child)\n", rc);
    printf("\t kill -CONT %d (Resumes the child)\n", rc);
    printf("\t kill -TERM %d (Kills the child)\n", rc);

    while (1) {
      pid_t result = waitpid(rc, &status, WUNTRACED | WCONTINUED);

      if (result == -1) {
        perror("waitpid failed\n");
        exit(1);
      }

      if (WIFEXITED(status)) {
        printf("[Parent] Child exited normally with code %d.\n",
               WEXITSTATUS(status));
        break;  // Exit the loop, child is dead
      } else if (WIFSIGNALED(status)) {
        printf("[Parent] Child was forefully KILLED by signal %d.\n",
               WTERMSIG(status));
        break;  // Exit the loop, child is dead.
      } else if (WIFSTOPPED(status)) {
        printf("[Parent] Child was PAUSED by signal %d.\n", WSTOPSIG(status));
      } else if (WIFCONTINUED(status)) {
        printf("[Parent] Child was RESUMED. \n");
      }
    }
  }

  return 0;
}