#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t create_process(char *buffer);
char **parse_line_to_argv(char *line);
char **add_path(char **search_paths, int MAX_PATHS, int num_paths,
                char *new_path);
void free_argv(char **argv);
void throw_error(char *message);

int main(int argc, char *argv[]) {
  // SEARCH PATHS
  int MAX_PATHS = 4;
  int num_paths = 0;
  char **search_paths = malloc(MAX_PATHS * sizeof(char *));

  // COMMANDS
  char *buffer = NULL;
  while (true) {
    printf("wish> ");
    buffer = NULL;
    size_t size = 0;
    int read = 0;
    read = getline(&buffer, &size, stdin);

    int child_pid = create_process(buffer);
    printf("\n");
  }
  free_argv(search_paths);
  free(buffer);

  return 0;
}

pid_t create_process(char *buffer) {
  char **argv = parse_line_to_argv(buffer);

  if (argv[0] == NULL) {
    free_argv(argv);
    return -1;
  }

  if (strcmp("exit", argv[0]) == 0) {
    if (argv[1] != NULL) {
      throw_error("exit doesn't take any arguments");
      free_argv(argv);
      return -1;
    }
    free_argv(argv);
    exit(0);
  } else if (strcmp("cd", argv[0]) == 0) {
    if (argv[1] == NULL || argv[2] != NULL) {
      throw_error("cd takes exactly one argument");
    } else {
      if (chdir(argv[1]) != 0) {
        throw_error("chdir failed");
      }
    }
    free_argv(argv);
    return -1;
  }

  pid_t child_pid = fork();

  if (child_pid < 0) {
    fprintf(stderr, "Fork failed.\n");
    exit(1);
  } else if (child_pid == 0) {
    char full_path[PATH_MAX];
    snprintf(full_path, sizeof(full_path), "/usr/%s", argv[0]);

    if (access(full_path, X_OK) == 0) {
      execv(full_path, argv);
    } else {
      snprintf(full_path, sizeof(full_path), "/bin/%s", argv[0]);
      execv(full_path, argv);
    }

    perror("Command not found");
    free_argv(argv);
    exit(1);
  } else {
    int child_wait = waitpid(child_pid, NULL, 0);
    free_argv(argv);
  }
  return child_pid;
}

char **parse_line_to_argv(char *line) {
  int capacity = 4;
  int argc = 0;

  char **argv = malloc(capacity * sizeof(char *));
  if (!argv) {
    perror("Failed to allocate argv");
    return NULL;
  }

  const char *delimiters = " \t\n";
  char *token;

  char *runner = line;

  while ((token = strsep(&runner, delimiters)) != NULL) {
    if (*token == '\0') {
      continue;
    }

    if (argc >= capacity - 1) {
      capacity = capacity * 2;
      char **temp = realloc(argv, capacity * sizeof(char *));
      if (!temp) {
        perror("Failed to realloc argv");
        free_argv(argv);
        return NULL;
      }
      argv = temp;
    }

    argv[argc] = strdup(token);
    if (!argv[argc]) {
      perror("Failed to strdup token");
      free_argv(argv);
      return NULL;
    }
    argc++;
  }

  argv[argc] = NULL;
  return argv;
}

char **add_path(char **search_paths, int MAX_PATHS, int num_paths,
                char *new_path) {
  if (num_paths >= MAX_PATHS - 1) {
    MAX_PATHS *= 2;
    char **temp = realloc(search_paths, MAX_PATHS * sizeof(char *));
    if (!temp) {
      free_argv(search_paths);
      return NULL;
    }
  }
}

void free_argv(char **argv) {
  for (int i = 0; argv[i] != NULL; i++) {
    free(argv[i]);
  }
  free(argv);
}

void throw_error(char *message) {
  char error_message[256];

  snprintf(error_message, sizeof(error_message), "An error has occurred: %s\n",
           message);
  write(STDERR_FILENO, error_message, strlen(error_message));
}