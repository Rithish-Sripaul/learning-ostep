#include <fcntl.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t create_process(char** argv, char** search_paths, int num_paths);
int check_built_in_commands(char** argv, char* buffer, char** search_paths,
                            int* max_paths, int* num_paths);
char** create_new_path(char** search_paths, int* max_paths, int* num_paths,
                       char* new_path);
char** parse_line_to_argv(char* buffer);
char* parse_to_redirection(char* buffer);
void throw_error(char* message);
void free_memory(char** arr);

int main(int argc, char* argv[]) {
  int saved_stdout = dup(STDOUT_FILENO);
  int max_paths = 4;
  int num_paths = 0;
  char** search_paths = malloc(max_paths * sizeof(char*));
  create_new_path(search_paths, &max_paths, &num_paths, "/usr/bin/");

  char* buffer = NULL;
  size_t size = 0;
  int read;

  bool read_from_file = false;
  FILE* read_stream = stdin;
  if (argc > 1) {
    read_stream = fopen(argv[1], "r");
    if (!read_stream) {
      throw_error("File could not be opened.");
      free(buffer);
      exit(1);
    }
    argv[0];
    read_from_file = true;
  }

  while (true) {
    if (!read_from_file) printf("wish> ");

    read = getline(&buffer, &size, read_stream);
    if (read == -1) {
      free(buffer);
      if (read_from_file) fclose(read_stream);
      exit(0);
    }

    char** my_argv;
    buffer = parse_to_redirection(buffer);
    my_argv = parse_line_to_argv(buffer);

    if (check_built_in_commands(my_argv, buffer, search_paths, &max_paths,
                                &num_paths) == 1) {
      free_memory(my_argv);
      printf("\n");
      continue;
    }

    pid_t child_process = create_process(my_argv, search_paths, num_paths);

    dup2(saved_stdout, STDOUT_FILENO);
    free_memory(my_argv);
    printf("\n");
  }

  if (read_from_file) fclose(read_stream);
  free(buffer);
  close(saved_stdout);
  return 0;
}

pid_t create_process(char** argv, char** search_paths, int num_paths) {
  if (num_paths == 0) {
    throw_error("No paths specified");
    return -1;
  }
  pid_t child_pid = fork();

  if (child_pid < 0) {
    throw_error("Fork failed.");
    exit(1);
  } else if (child_pid == 0) {
    char full_path[PATH_MAX];
    int i;
    for (i = 0; i < num_paths; i++) {
      snprintf(full_path, sizeof(full_path), "%s%s", search_paths[i], argv[0]);
      if (access(full_path, X_OK) != -1) {
        break;
      }
    }
    if (i < num_paths) {
      execv(full_path, argv);
    }

    perror("Command not found");
    exit(1);
  } else {
    int child_wait = waitpid(child_pid, NULL, 0);
  }
  return child_pid;
}

int check_built_in_commands(char** argv, char* buffer, char** search_paths,
                            int* max_paths, int* num_paths) {
  if (argv[0] == NULL) {
    return 1;
  }

  // ---- IN BUILT COMMANDS ----
  if (strcmp("exit", argv[0]) == 0) {
    if (argv[1] != NULL) {
      throw_error("exit takes no arguments");
      return 1;
    }
    free(buffer);
    free_memory(argv);
    exit(0);
  } else if (strcmp("cd", argv[0]) == 0) {
    if (argv[1] == NULL || argv[2] != NULL) {
      throw_error("cd takes exactly one argument");
      return 1;
    }

    int rc = chdir(argv[1]);
    if (rc == -1) {
      perror("cd failed");
      return 1;
    }
    return 1;
  } else if (strcmp("path", argv[0]) == 0) {
    printf("Creating new path\n");
    for (int i = 1; argv[i] != NULL; i++) {
      search_paths =
          create_new_path(search_paths, max_paths, num_paths, argv[i]);
    }
    return 1;
  }

  return 0;
}

char** create_new_path(char** search_paths, int* max_paths, int* num_paths,
                       char* new_path) {
  // Memory Full
  if (*num_paths >= (*max_paths) - 1) {
    *max_paths *= 2;
    char** temp = realloc(search_paths, (*max_paths) * sizeof(char*));
    if (!temp) {
      throw_error("Realloc Failed.");
    }
    search_paths = temp;
  }

  // String Duplication
  search_paths[*num_paths] = strdup(new_path);
  if (!search_paths[*num_paths]) {
    throw_error("strdup failed");
  }
  (*num_paths)++;

  return search_paths;
}

char** parse_line_to_argv(char* buffer) {
  int capacity = 4;
  int argc = 0;

  char** argv = malloc(capacity * sizeof(char*));
  if (!argv) {
    throw_error("Fork failed.\n");
    exit(1);
  }

  char* token;
  char* delimiters = " \t\n";
  char* runner = buffer;
  while ((token = strsep(&runner, delimiters)) != NULL) {
    if (*token == '\0') {
      continue;
    }

    if (argc >= capacity - 1) {
      capacity *= 2;
      char** temp = realloc(argv, capacity * sizeof(char*));
      if (!temp) {
        throw_error("Realloc failed.");
        free_memory(argv);
        exit(1);
      }
      argv = temp;
    }

    argv[argc] = strdup(token);
    if (!argv[argc]) {
      throw_error("String Duplication failed.");
      free_memory(argv);
      exit(1);
    }
    argc++;
  }
  argv[argc] = NULL;
  return argv;
}

char* parse_to_redirection(char* buffer) {
  char* runner = buffer;
  char* token;
  token = strsep(&runner, ">");

  // NO REDIRECTION found in Buffer
  if (runner == NULL) {
    return token;
  }

  char* file_name = strsep(&runner, ">");
  int file_desc = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (file_desc < 0) {
    throw_error("Error opening file");
    free(buffer);
    exit(1);
  }

  if (dup2(file_desc, STDOUT_FILENO) < 0) {
    throw_error("Error executing dup2");
    free(buffer);
    exit(1);
  }

  close(file_desc);
  return token;
}

void throw_error(char* message) {
  char error_message[256];
  snprintf(error_message, sizeof(error_message), "Error: %s\n", message);
  write(STDERR_FILENO, error_message, strlen(error_message));
  return;
}

void free_memory(char** arr) {
  for (int i = 0; arr[i] != NULL; i++) {
    free(arr[i]);
  }
  free(arr);
}