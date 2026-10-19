#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

char **parse_line_to_command(char *buffer);
void free_commands(char **commands);
pid_t create_process(char *buffer);

int main(int argc, char *argv[]) {
  // Input
  while (true) {
    printf("wish> ");
    char *buffer;
    size_t size = 0;
    getline(&buffer, &size, stdin);
    char **commands = parse_line_to_command(buffer);

    free(commands);
  }
  return 0;
}

pid_t create_process(char *buffer) {}

char **parse_line_to_command(char *buffer) {
  printf("I'm here\n");
  int total_num_of_commands = 4;
  int curr_num_of_commands = 0;
  char **commands = malloc(total_num_of_commands * sizeof(char *));

  char *delimiters = " \t\n";
  char *word;

  while ((word = strsep(&buffer, delimiters)) != NULL) {

    if (curr_num_of_commands >= total_num_of_commands - 1) {
      total_num_of_commands *= 2;
      char **temp = realloc(commands, total_num_of_commands * sizeof(char *));
      if (!temp) {
        perror("Failed to realloc commands");
        free_commands(commands);
        return NULL;
      }
      commands = temp;
    }

    commands[curr_num_of_commands] = strdup(word);
    if (!commands[curr_num_of_commands]) {
      perror("Faile to strdup");
      free_commands(commands);
      return NULL;
    }
    curr_num_of_commands++;
  }

  commands[curr_num_of_commands] = NULL;
  return commands;
}

void free_commands(char **commands) {
  for (int i = 0; commands[i] != NULL; i++) {
    free(commands[i]);
  }
  free(commands);
}