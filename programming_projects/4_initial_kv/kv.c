#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_LEN 256

struct node {
  int number;
  char name[NAME_LEN + 1];
  struct node* next;
};

void put(struct node** data, int number, char name[]);
void get(struct node* data, int number);
void delete(struct node** data, int number);
void clear(struct node** data);
void display(struct node* data);
struct node* create_node(int number, char name[]);
void load_from_database(FILE* fp, struct node** data);
void write_to_database(FILE* fp, struct node* data);
void free_nodes(struct node* data);

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return 0;
  }

  struct node* data = NULL;
  FILE* fp = fopen("database.txt", "r");
  if (fp != NULL) {
    load_from_database(fp, &data);
    fclose(fp);
  }

  char code;
  int number;
  char name[NAME_LEN + 1];
  for (int i = 1; i < argc; i++) {
    sscanf(argv[i], "%c,%d,%s", &code, &number, name);
    switch (code) {
      case 'p':
        put(&data, number, name);
        break;
      case 'g':
        get(data, number);
        break;
      case 'd':
        delete(&data, number);
        break;
      case 'c':
        clear(&data);
        break;
      case 'a':
        display(data);
        break;
      default:
        printf("Invalid code.\n");
        break;
    }
  }
  fp = fopen("database.txt", "w");
  write_to_database(fp, data);
  fclose(fp);
  free_nodes(data);
  return 0;
}

// --------------------------------------
// ----------- KV FUNCTIONS -----------
// --------------------------------------

void put(struct node** data, int number, char name[]) {
  struct node* new_node = create_node(number, name);
  if (*data == NULL) {
    *data = new_node;
    return;
  }
  struct node* p;

  for (p = *data; p->next != NULL; p = p->next);

  p->next = new_node;
  return;
}

void get(struct node* data, int number) {
  struct node* p;
  for (p = data; p != NULL; p = p->next) {
    if (p->number == number) {
      printf("%d,%s\n", number, p->name);
      return;
    }
  }
  printf("%d not found.\n", number);
  return;
}

void delete(struct node** data, int number) {
  struct node *current, *prev;
  for (current = *data, prev = NULL;
       current != NULL && current->number != number;
       prev = current, current = current->next);

  // Element not found
  if (current == NULL) {
    printf("%d not found.\n", number);
    return;
  }
  // First element to be deleted
  if (prev == NULL) {
    *data = current->next;
  } else {
    prev->next = current->next;
  }

  free(current);
  return;
}

void clear(struct node** data) {
  free_nodes(*data);
  *data = NULL;
  return;
}

void display(struct node* data) {
  struct node* p;
  printf("No.\tName\n");
  for (p = data; p != NULL; p = p->next) {
    printf("%d\t%s\n", p->number, p->name);
  }
  printf("\n");
}

// --------------------------------------
// ----------- NODE FUNCTIONS -----------
// --------------------------------------

struct node* create_node(int number, char name[]) {
  struct node* new_node = malloc(sizeof(struct node));

  if (new_node == NULL) {
    printf("Error: malloc failed at create node.");
    exit(EXIT_FAILURE);
  }
  new_node->number = number;
  strcpy(new_node->name, name);
  new_node->next = NULL;
  return new_node;
}

void free_nodes(struct node* data) {
  struct node* current = data;
  struct node* next_node;

  while (current != NULL) {
    next_node = current->next;
    free(current);
    current = next_node;
  }
}

// --------------------------------------
// ------- DATABASE FUNCTIONS -----------
// --------------------------------------

void load_from_database(FILE* fp, struct node** data) {
  char buffer[512];
  int number;
  char name[NAME_LEN + 1];
  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    sscanf(buffer, "%d,%256s\n", &number, name);
    put(data, number, name);
  }
}

void write_to_database(FILE* fp, struct node* data) {
  struct node* p;
  char buffer[512];
  for (p = data; p != NULL; p = p->next) {
    snprintf(buffer, sizeof(buffer), "%d,%s\n", p->number, p->name);
    fputs(buffer, fp);
  }
}