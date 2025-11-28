#include <stdio.h>
#include <stdlib.h>

#include "util.h"

char *util_read_file(const char *file) {
  FILE *f = fopen(file, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET); // same as rewind(f);

  char *text = (char *)malloc(fsize + 1);
  if (fread(text, fsize, 1, f)) {
    int errcode = ferror(f);
    if (errcode) {
      printf("Error: reading file returned error %d\n", errcode);
    }
  }

  fclose(f);

  text[fsize] = 0;
  return text;
}

int util_imax(int x, int y) {
  if (x > y)
    return x;
  else
    return y;
}

int util_imin(int x, int y) {
  if (x < y)
    return x;
  else
    return y;
}

struct linked_list *util_list_create() {
  struct linked_list *list =
      (struct linked_list *)malloc(sizeof(struct linked_list));
  list->count = 0;
  list->head = NULL;
  return list;
}

void util_list_add(struct linked_list *list, void *item) {
  if (list == NULL)
    return;

  struct list_node *node;

  node = (struct list_node *)malloc(sizeof(struct list_node));
  node->item = item;
  node->next = list->head;
  list->head = node;
  list->count++;
}

void util_list_remove(struct linked_list *list, void *item) {
  if (list == NULL)
    return;

  struct list_node *node = list->head;

  while (node != NULL) {
    if (node->next != NULL && node->next->item == item) {
      node->next = node->next->next;
      free(node->next);
      break;
    }
    node = node->next;
  }
}

struct queue util_queue_create() {
  struct queue result;
  result.count = 0;
  result.front = result.back = NULL;
  return result;
}

void util_queue_push(struct queue *queue, void *item) {
  struct list_node *node;
  queue->count++;

  node = (struct list_node *)malloc(sizeof(struct list_node));
  node->item = item;
  node->next = NULL;

  if (queue->front == NULL) {
    queue->front = queue->back = node;
  } else {
    queue->front->next = node;
    queue->front = node;
  }
}

void *util_queue_pop(struct queue *queue) {
  struct list_node *tmp;
  void *result;

  if (queue->back == NULL) {
    result = NULL;
  } else {
    queue->count--;
    tmp = queue->back;
    result = queue->back->item;
    queue->back = queue->back->next;
    free(tmp);
  }

  return result;
}

void util_queue_free(struct queue *queue) {
  struct list_node *node = queue->back;
  while (node != NULL) {
    free(node);
    node = node->next;
  }
  queue->back = queue->front = NULL;
  queue->count = 0;
}
