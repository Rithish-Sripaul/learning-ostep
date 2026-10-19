#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork failed.\n");
    exit(1);
  } else if (pid == 0) {
    printf("[CHILD] PID is %d. I am running\n", (int)getpid());

    while (1) {
      printf("[Child] still working...");
      sleep(1);
    }
  } else {
    int status;
    printf("[Parent] Monitoring child process %d.\n", pid);
    printf("\n---> Open another terminal window and try these commands:\n");
    printf("     kill -STOP %d   (Pauses the child)\n", pid);
    printf("     kill -CONT %d   (Resumes the child)\n", pid);
    printf("     kill -TERM %d   (Kills the child)\n\n", pid);

    while (1) {
      pid_t result = waitpid(pid, &status, WUNTRACED | WCONTINUED);

      if (result == -1) {
        perror("waitpid failed.\n");
        exit(1);
      }

      if (WIFEXITED(status)) {
        printf("[PARENT] Child exited normallu with code %d.\n",
               WEXITSTATUS(status));
        break;
      } else if (WIFSIGNALED(status)) {
        printf("[PARENT] Child was forecefully KILLED by signal %d.\n",
               WTERMSIG(status));
        break;
      } else if (WIFSTOPPED((status))) {
        printf("[PARENT] Child was PAUSED by signal %d\n", WSTOPSIG(status));
      } else if (WIFCONTINUED(status)) {
        printf("[PARENT] Child was RESUMED");
      }
    }
  }

  return 0;
  b
}