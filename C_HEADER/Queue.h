#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define int_f 0b000001
#define char_f 0b000010
#define float_f 0b000100
#define str_f 0b001000
#define long_f 0b010000
#define double_f 0b100000

typedef struct node {
  int flag;
  void *data;
  struct node *next;
} Node;

typedef struct queue {
  int len;
  Node *start;
  Node *end;
} Queue;

Queue *init_queue() {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  if (queue == NULL) {
    return NULL;
  }
  queue->end = queue->start = NULL;
  queue->len = 0;
}

void enque(Queue *queue, void *data, int flag) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return;
  }
  node->flag = 1 << flag;
  node->data = data;
  node->next = NULL;
  if (queue->start == NULL) {
    queue->start = node;
    queue->end = node;
    queue->len++;
    return;
  }
  queue->end->next = node;
  queue->end = node;
  queue->len++;
  return;
}

void deque(Queue *queue) {
  if (queue->start == NULL) {
    printf("Queue is empty\n");
    return;
  }

  if (queue->len == 1) {
    free(queue->start);
    queue->len--;
    queue->start = queue->end = NULL;
    return;
  }
  Node *temp = queue->start;
  queue->start = queue->start->next;
  queue->len--;
  free(temp);
}

int is_empty(Queue *queue) { return queue->start == NULL ? 1 : 0; }

void print_queue(Queue *queue) {
  printf("< START >");
  Node *node = queue->start;
  if (node == NULL) {
    printf(" -> < END >\n ");
    return;
  }
  for (int i = 0; i < queue->len; i++) {
    printf(" -> ");
    if (node->flag == int_f) {
      printf("< %d >", *(int *)(node->data));
    } else if (node->flag == char_f) {
      printf("< %c >", *(char *)(node->data));
    } else if (node->flag == float_f) {
      printf("< %f >", *(float *)(node->data));
    } else if (node->flag == str_f) {
      printf("< %s >", (char *)(node->data));
    } else if (node->flag == long_f) {
      printf("< %ld >", *(long *)(node->data));
    } else if (node->flag == double_f) {
      printf("< %lf >", *(double *)(node->data));
    } else {
    }
    node = node->next;
  }
  printf(" -> < END >\n");
  return;
}

void deallocate_queue(Queue **queue) {
  while ((*queue)->start != NULL) {
    deque(*queue);
  }
  (*queue)->start = (*queue)->end = NULL;
  return;
}
