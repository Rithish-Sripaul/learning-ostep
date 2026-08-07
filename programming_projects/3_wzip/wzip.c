#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* buffer = NULL;
char curr_char;
int count = 0;
int main(int argc, char* argv[]) {
  for (int k = 1; k < argc; k++) {
    FILE* fp = fopen(argv[k], "r");
    if (fp == NULL) {
      printf("Cannot open file.\n");
      exit(1);
    }

    size_t size = 0;
    int i = 0;
    while ((i = getline(&buffer, &size, fp)) != -1) {
      if (!curr_char) {
        curr_char = buffer[0];
      }
      for (int j = 0; j < i; j++) {
        if (curr_char != buffer[j]) {
          fwrite(&count, sizeof(int), 1, stdout);
          fwrite(&curr_char, sizeof(char), 1, stdout);

          curr_char = buffer[j];
          count = 0;
        }
        count++;
      }
    }

    fclose(fp);
  }
  fwrite(&count, sizeof(int), 1, stdout);
  fwrite(&curr_char, sizeof(char), 1, stdout);
  free(buffer);
  return 0;
}