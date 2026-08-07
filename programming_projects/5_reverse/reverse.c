#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  char* value;
  struct node* prev;
  struct node* next;
};

void add_node(struct node** data, char* buffer);
struct node* create_node(char* buffer);
void read_input_file(struct node** data, FILE* fp);
void write_output(struct node** data, FILE* fp);
void free_nodes(struct node* data);
void display(struct node* data);
void display_reverse(struct node* data);

int main(int argc, char* argv[]) {
  if (argc > 2) {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
  }
  // else if (strcmp(argv[1], argv[2]) == 0) {
  //   fprintf(stderr, "Input and output file must differ.\n");
  //   exit(1);
  // }

  struct node* data = NULL;

  FILE* input_fp = fopen("input.txt", "r");
  read_input_file(&data, input_fp);
  fclose(input_fp);

  FILE* output_fp = fopen("output.txt", "w");
  write_output(&data, output_fp);
  fclose(output_fp);
  free_nodes(data);
  return 0;
}

void add_node(struct node** data, char* buffer) {
  struct node* new_node = create_node(buffer);
  if (*data == NULL) {
    *data = new_node;
    return;
  }

  struct node* p;
  for (p = *data; p->next != NULL; p = p->next);
  p->next = new_node;
  new_node->prev = p;
  return;
}

struct node* create_node(char* buffer) {
  struct node* new_node = malloc(sizeof(struct node));

  new_node->value = strdup(buffer);
  new_node->next = NULL;
  new_node->prev = NULL;
  return new_node;
}
void display(struct node* data) {
  struct node* p;
  for (p = data; p != NULL; p = p->next) {
    printf("%s", p->value);
  }
}

void display_reverse(struct node* data) {
  struct node* p;

  for (p = data; p->next != NULL; p = p->next) {
  }

  for (; p != NULL; p = p->prev) {
    printf("%s", p->value);
  }
}

void read_input_file(struct node** data, FILE* fp) {
  char* buffer = NULL;
  size_t size = 0;
  int numChar;
  while ((numChar = getline(&buffer, &size, fp)) != -1) {
    add_node(data, buffer);
  }
  free(buffer);
}

void write_output(struct node** data, FILE* fp) {
  struct node* p;

  for (p = *data; p->next != NULL; p = p->next) {
  }

  for (; p != NULL; p = p->prev) {
    fputs(p->value, fp);
  }
}

void free_nodes(struct node* data) {
  struct node* current = data;
  struct node* next;

  while (current != NULL) {
    next = current->next;
    free(current->value);
    free(current);
    current = next;
  }
  return;
}
