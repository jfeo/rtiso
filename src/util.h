#ifndef __RTISO_UTIL_H__
#define __RTISO_UTIL_H__

char *util_read_file(const char *file);

int util_imax(int a, int b);
int util_imin(int a, int b);

// data structures

// linked list

struct linked_list {
  struct list_node *head;
  unsigned int count;
};

struct list_node {
  void *item;
  struct list_node *next;
};

struct linked_list *util_list_create();
void util_list_add(struct linked_list *list, void *item);
void util_list_remove(struct linked_list *list, void *item);

// queue
struct queue {
  struct list_node *front;
  struct list_node *back;
  unsigned int count;
};

struct queue util_queue_create();
void util_queue_push(struct queue *queue, void *item);
void *util_queue_pop(struct queue *queue);
void util_queue_free(struct queue *queue);

#endif /* ifndef __RTISO_UTIL_H__ */
