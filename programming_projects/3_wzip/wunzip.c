#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    printf("wunzip:file1[file2...]\n");
    exit(1);
  }

  for (int k = 1; k < argc; k++) {
    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL) {
      printf("Cannot open file.\n");
      exit(1);
    }

    int count;
    char chr;

    while (fread(&count, sizeof(int), 1, fp) != 0) {
      fread(&chr, sizeof(char), 1, fp);
      for (int i = 0; i < count; i++) {
        printf("%c", chr);
      }
    }

    fclose(fp);
  }

  return 0;
}