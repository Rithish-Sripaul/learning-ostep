#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  // 1. Check if the search term was provided
  if (argc <= 1) {
    printf("wgrep: searchterm [file ...]\n");
    exit(1);
  }

  // 2. If no files are provided (argc == 2), we loop once and read from stdin.
  // Otherwise, we loop through the provided files.
  int num_files = (argc == 2) ? 1 : (argc - 2);

  for (int i = 0; i < num_files; i++) {
    // Default to stdin
    FILE* fp = stdin;

    // If files were provided, attempt to open them
    if (argc > 2) {
      fp = fopen(argv[i + 2], "r");
      if (fp == NULL) {
        printf("wgrep: cannot open file\n");
        exit(1);
      }
    }

    char* buffer = NULL;
    size_t size = 0;  // Initialize to 0 for getline

    // 3. Read line by line
    while (getline(&buffer, &size, fp) != -1) {
      if (strstr(buffer, argv[1]) != NULL) {
        printf("%s", buffer);
      }
    }

    // 4. Clean up
    if (fp != stdin) {
      fclose(fp);
    }
    free(buffer);
  }

  return 0;
}
