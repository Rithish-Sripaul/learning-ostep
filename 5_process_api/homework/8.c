#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    fprintf(stderr, "Pipe failed.\n");
    exit(1);
  }

  int child_1 = fork();
  if (child_1 < 0) {
    fprintf(stderr, "Child not created.\n");
    exit(1);
  } else if (child_1 == 0) {
    close(pipefd[0]);

    dup2(pipefd[1], STDOUT_FILENO);

    close(pipefd[1]);

    printf("Hello World.\n");
    exit(0);
  }

  int child_2 = fork();
  if (child_2 < 0) {
    fprintf(stderr, "Child not created.\n");
    exit(1);
  } else if (child_2 == 0) {
    close(pipefd[1]);

    dup2(pipefd[0], STDIN_FILENO);

    close(pipefd[0]);

    char buffer[256];

    int bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
      buffer[bytes_read] = '\0';
      fprintf(stderr, "[Child 2 Received]: %s\n", buffer);
    }
    exit(0);
  }

  close(pipefd[0]);
  close(pipefd[1]);

  waitpid(child_1, NULL, 0);
  waitpid(child_2, NULL, 0);

  return 0;
}