#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NAME_LEN 256

struct node {
  int number;
  char name[NAME_LEN + 1];
  struct node *next;
};

struct node *create_node(int number, char name[]);
void put(struct node **data, int number, char name[]);
void get(struct node *data, int number);
void delete(struct node **data, int number);
void clear(struct node **data);
void all(struct node *data);
void load_from_database(FILE *fp, struct node **data);
void write_to_database(FILE *fp, struct node *data);
void free_nodes(struct node *data);

int main(int argc, char *argv[]) {
  FILE *fp = fopen("database.txt", "r");
  if (fp == NULL) {
    printf("database failed to open.\n");
    exit(1);
  }

  struct node *data = NULL;
  load_from_database(fp, &data);
  fclose(fp);

  char action;
  int number;
  char name[NAME_LEN + 1];

  for (int commands = 1; commands < argc; commands++) {
    sscanf(argv[commands], "%c,%d,%s", &action, &number, name);

    switch (action) {
    case 'a':
      all(data);
      break;
    case 'p':
      put(&data, number, name);
      break;
    case 'g':
      get(data, number);
      break;
    case 'd':
      delete (&data, number);
      break;
    case 'c':
      clear(&data);
      break;
    }
  }
  fp = fopen("database.txt", "w");
  write_to_database(fp, data);
  fclose(fp);
  free_nodes(data);
  return 0;
}

struct node *create_node(int number, char name[]) {
  struct node *new_node = malloc(sizeof(struct node));

  if (new_node == NULL) {
    printf("malloc failed.\n");
    exit(1);
  }

  new_node->number = number;
  strcpy(new_node->name, name);
  new_node->next = NULL;

  return new_node;
}

void put(struct node **data, int number, char name[]) {
  struct node *new_node = create_node(number, name);

  if (*data == NULL) {
    *data = new_node;
    return;
  }

  struct node *p;
  for (p = *data; p->next != NULL; p = p->next)
    ;

  p->next = new_node;
  return;
}

void all(struct node *data) {
  struct node *p;
  for (p = data; p != NULL; p = p->next) {
    printf("%d,%s\n", p->number, p->name);
  }
  return;
}

void get(struct node *data, int number) {
  struct node *p;
  for (p = data; p != NULL; p = p->next) {
    if (p->number == number) {
      printf("%d,%s\n", p->number, p->name);
      return;
    }
  }
  printf("%d not found\n", number);
  return;
}

void delete(struct node **data, int number) {
  struct node *current = *data, *prev;
  for (current = *data, prev = NULL;
       current != NULL && current->number != number;
       prev = current, current = current->next) {
  }

  // Element not found
  if (current == NULL) {
    printf("%d not found\n", number);
    return;
  }

  // Element is first
  if (prev == NULL) {
    *data = current->next;
  } else {
    prev->next = current->next;
  }
  free(current);
  return;
}

void clear(struct node **data) {
  struct node *current, *next;
  current = *data;

  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }

  *data = NULL;
  return;
}

void load_from_database(FILE *fp, struct node **data) {
  size_t size = 0;
  char *buffer;

  while (getline(&buffer, &size, fp) != -1) {
    int number;
    char name[NAME_LEN + 1];

    sscanf(buffer, "%d,%s", &number, name);
    put(data, number, name);
  }

  return;
}

void write_to_database(FILE *fp, struct node *data) {
  struct node *p;
  for (p = data; p != NULL; p = p->next) {
    fprintf(fp, "%d,%s\n", p->number, p->name);
  }
}

void free_nodes(struct node *data) {
  struct node *current = data;
  struct node *next;
  while (current) {
    next = current->next;
    free(current);
    current = next;
  }
}