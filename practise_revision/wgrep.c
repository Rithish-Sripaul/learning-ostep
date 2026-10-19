#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("wgrep: searchterm [file ...]\n");
    exit(1);
  }

  if (argc == 2) {
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
      if (strstr(buffer, argv[1]) != NULL) {
        printf("%s", buffer);
      }
    }
    exit(0);
  }

  for (int file_no = 2; file_no < argc; file_no++) {
    FILE *fp = fopen(argv[file_no], "r");

    if (fp == NULL) {
      printf("wgrep: cannot open file\n");
      exit(1);
    }

    size_t n = 0;
    char *buffer;

    while (getline(&buffer, &n, fp) != -1) {
      if (strstr(buffer, argv[1]) != NULL) {
        printf("%s", buffer);
      }
    }
    fclose(fp);
  }

  exit(0);
}