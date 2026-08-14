#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int fd = open("./test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    fprintf(stderr, "Error opening file.\n");
    exit(1);
  }

  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork failed.\n");
    exit(1);
  } else if (rc == 0) {
    const char* text =
        "Hello, this is written using open() and write(). CHILD\n";

    ssize_t bytes_written = write(fd, text, strlen(text));

    if (bytes_written == -1) {
      fprintf(stderr, "Erorr writing to file. \n");
      exit(1);
    }

    printf("Child process: (pid: %d)\n", (int)getpid());
    printf("child: file desc: %d\n", fd);
  } else {
    const char* text =
        "Hello, this is written using open() and write(). PARENt\n";

    // int rc_wait = wait(NULL);
    ssize_t bytes_written = write(fd, text, strlen(text));

    if (bytes_written == -1) {
      fprintf(stderr, "Erorr writing to file. \n");
      exit(1);
    }
    printf("parent: file desc: %d\n", fd);
  }
  close(fd);
  return 0;
}