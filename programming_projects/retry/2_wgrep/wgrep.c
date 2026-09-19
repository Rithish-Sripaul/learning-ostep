#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[]) {
  if (argc >= 2) {
    FILE* fp = fopen(argv[argc - 1], "r");
    char* buffer = NULL;
    size_t size = 0;

    while (getline(&buffer, &size, fp) != -1) {
      if (strstr(buffer, argv[1]) != NULL) {
        printf("%s", buffer);
      }
    }
    fclose(fp);
  }
  return 0;
}