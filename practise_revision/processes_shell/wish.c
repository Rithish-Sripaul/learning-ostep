#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
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
    create_process(buffer);
  }
  return 0;
}

pid_t create_process(char *buffer) {
  char **commands = parse_line_to_command(buffer);

  // checking for built in commands
  if (strstr(*commands, "exit")) {
    free_commands(commands);
    exit(0);
  }

  pid_t child_pid = fork();

  if (child_pid == -1) {
    perror("Fork failes");
    exit(1);
  } else if (child_pid == 0) {
    // Executing commands
    char full_path[PATH_MAX];
    snprintf(full_path, sizeof(full_path), "/bin/%s", commands[0]);
    if (access(full_path, X_OK) == 0) {
      execv(full_path, commands);
    } else {
      snprintf(full_path, sizeof(full_path), "/usr/%s", commands[0]);
      execv(full_path, commands);
    }
  } else {
    waitpid(child_pid, NULL, 0);
    free_commands(commands);
  }
  return child_pid;
}

char **parse_line_to_command(char *buffer) {
  int total_num_of_commands = 4;
  int curr_num_of_commands = 0;
  char **commands = malloc(total_num_of_commands * sizeof(char *));

  char *delimiters = " \t\n";
  char *word;

  while ((word = strsep(&buffer, delimiters)) != NULL) {
    if (*word == '\0') {
      continue;
    }

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