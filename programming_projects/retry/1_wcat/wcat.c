#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    FILE* fp = fopen(argv[i], "r");

    if (fp == NULL) {
      printf("Cannot open file.\n");
      exit(1);
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
      printf("%s", buffer);
    }
    printf("\n");
    fclose(fp);
  }
  return 0;
}