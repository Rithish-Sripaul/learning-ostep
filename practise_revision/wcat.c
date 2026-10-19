
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    return 0;
  }

  for (int file_no = 1; file_no < argc; file_no++) {
    FILE *fp = fopen(argv[file_no], "r");
    if (fp == NULL) {
      perror("wcat: cannot open file\n");
      exit(0);
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
      printf("%s", buffer);
    }
    fclose(fp);
  }
  return 0;
}