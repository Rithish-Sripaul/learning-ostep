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

struct node* create_node(char* buffer) {
  struct node* new_node = malloc(sizeof(struct node));
  new_node->value = buffer;
  new_node->next = NULL;
  new_node->prev = NULL;
  return new_node;
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

void read_input_file(struct node** data, FILE* fp) { FILE* fp = return; }