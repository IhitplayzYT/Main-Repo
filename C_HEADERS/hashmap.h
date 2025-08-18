#ifndef HASH
#define HASH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif

typedef struct node_h {
  struct node_h *next;
  struct node_h *prev;
  void *data;
  int flag;
  int len;
} Node_h;

Node_h **init_hash_table(int m) {
  Node_h **arr = (Node_h **)malloc(sizeof(Node_h *) * m);
  if (arr == NULL) {
    return NULL;
  }
  for (int i = 0; i < m; i++) {
    arr[i] = (Node_h *)malloc(sizeof(Node_h));
    arr[i]->data = arr[i]->next = arr[i]->prev = NULL;
    arr[i]->len = m;
    arr[i]->flag = 0;
  }
  return arr;
}

int compute_hash(char *data) {
  int l = strlen(data);
  int hash = 5381;
  for (int i = 0; i < l; i++) {
    hash = ((hash << 5) + hash) + data[i];
  }
  return hash;
}

void add_to_table(Node_h **hashtable, void *val, int flag) {
  if (flag == 0) {
    Node_h *ptr = hashtable[*((int *)val) % hashtable[0]->len];
    if (ptr->data == NULL) {
      ptr->data = (int *)val;
      ptr->flag = 0;
      ptr->next = NULL;
      return;
    }
    while (ptr->next != NULL) {
      if (ptr->data == NULL) {
        break;
      }
      ptr = ptr->next;
    }
    ptr->next = (Node_h *)malloc(sizeof(Node_h));
    ptr->next->flag = 0;
    ptr->next->data = (int *)val;
    ptr->next->prev = ptr;
  } else {
    int l = hashtable[0]->len;
    int z = compute_hash((char *)val) % l;
    Node_h *ptr = hashtable[z];
    if (ptr->data == NULL) {
      ptr->data = (char *)val;
      ptr->flag = 1;
      ptr->next = NULL;
      return;
    }

    while (ptr->next != NULL) {
      if (ptr->data == NULL) {
        break;
      }
      ptr = ptr->next;
    }
    ptr->next = (Node_h *)malloc(sizeof(Node_h));
    ptr->next->flag = 1;
    ptr->next->data = (char *)val;
    ptr->next->prev = ptr;
  }
}

void print_hash(Node_h *a) {
  Node_h *ptr = a;
  while (ptr != NULL) {
    if (ptr->data != NULL) {
      if (ptr->flag == 0) {
        printf("%d ", *(int *)ptr->data);
      } else {
        printf("%s ", (char *)ptr->data);
      }
    } else {
      printf("_ ");
    }
    ptr = ptr->next;
  }
}

void print_table(Node_h **a) {
  if (a == NULL) {
    return;
  }
  int k = a[0]->len;
  for (int i = 0; i < k; i++) {
    print_hash(a[i]);
    printf("\n");
  }
}

void *find_hash(Node_h **arr, void *data, int flag) {
  if (arr == NULL) {
    return NULL;
  }
  int k = 0;
  if (flag == 0) {
    int t = *(int *)data % arr[0]->len;
    Node_h *ptr = arr[t];
    while (ptr != NULL) {
      if (*(int *)ptr->data == *(int *)data) {
        k = 1;
        break;
      }
      ptr = ptr->next;
    }
    if (k == 1)
      return ptr->data;
  } else {
  }
  return NULL;
}

void clear_hash(Node_h *l) {
  if (l == NULL) {
    return;
  }
  l->data = NULL;
  l->flag = 0;
  l->len = 0;
  Node_h *ptr = l->next;
  l->next = NULL;
  if (ptr == NULL) {
    return;
  }
  while (l->next != NULL) {
    l = l->next;
    free(l->prev);
  }
  free(l);
}

void free_hashmap(Node_h **arr) {
  for (int i = 0; i < arr[0]->len; i++) {
    clear_hash(arr[i]);
  }
  free(arr);
  arr = NULL;
}
