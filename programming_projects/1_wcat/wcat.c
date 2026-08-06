#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {
  FILE* fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("Cannot open file.\n");
    exit(1);
  }

  char buffer[256];

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    printf("%s", buffer);
  }

  fclose(fp);
  return 0;
}