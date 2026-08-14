#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    fprintf(stderr, "Pipe failed.\n");
    exit(1);
  }

  pid_t child_1 = fork();
  if (child_1 < 0) {
    fprintf(stderr, "Fork failed.\n");
    exit(1);
  } else if (child_1 == 0) {
    printf("[Child 1] PID: %d\n", (int)getpid());
    close(pipefd[0]);

    dup2(pipefd[1], STDOUT_FILENO);

    close(pipefd[1]);

    printf("Hello world from Child 1 (pid: %d)", (int)getpid());

    exit(0);
  }

  pid_t child_2 = fork();
  if (child_2 < 0) {
    fprintf(stderr, "Fork failed.\n");
    exit(1);
  } else if (child_2 == 0) {
    printf("[Child 2] PID: %d\n", (int)getpid());

    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    char buffer[256];
    int bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

    if (bytes_read > 0) {
      buffer[bytes_read] = '\0';
      fprintf(stdout, "Child 2 received: %s\n", buffer);
    }
    exit(0);
  }

  close(pipefd[0]);
  close(pipefd[1]);

  waitpid(child_1, NULL, 0);
  waitpid(child_2, NULL, 0);

  return 0;
}