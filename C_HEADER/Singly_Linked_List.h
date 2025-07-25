#ifndef INCLUDES
#define INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif

#define int_f 0b000001
#define char_f 0b000010
#define float_f 0b000100
#define str_f 0b001000
#define long_f 0b010000
#define double_f 0b100000

/*typedef union data {
  int int_data;       //  pass 0
  char str_data[50];  // pass 1
  double double_data; // pass 2
  float float_data;   // pass 3
  char char_data;     // pass 4
  long long_data;     // pass 5
} u_data;*/

typedef struct Data {
  int flag;
  void *data;
} Data;

typedef struct Node {
  Data data;
  struct Node *next;
} Node;

int fill_data(Node **node, void *data) {
  switch ((*node)->data.flag) {
  case int_f:
    (*node)->data.data = ((int *)data);
    break;
  case char_f:
    (*node)->data.data = ((char *)data);
    break;
  case float_f:
    (*node)->data.data = ((float *)data);
    break;
  case str_f:
    (*node)->data.data = ((char *)data);
    break;
  case long_f:
    (*node)->data.data = ((long *)data);
    break;
  case double_f:
    (*node)->data.data = ((double *)data);
    break;
  default:
    return -1;
  }
  return 0;
}

Node *add_beggining(Node **head, void *data, int type) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return NULL;
  }
  node->data.flag = type = (1 << type);
  if (fill_data(&node, data) == -1) {
    printf("Error occured due to invalidtype provided");
    return NULL;
  }

  if (*head == NULL) {
    *head = node;
    node->next = NULL;
    return node;
  }
  node->next = *head;
  *head = node;
  return node;
}

Node *add_end(Node **head, void *data, int type) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return NULL;
  }
  node->data.flag = (1 << type);
  if (fill_data(&node, data) == -1) {
    printf("Error occured due to invalidtype provided");
    return NULL;
  }
  if (*head == NULL) {
    *head = node;
    node->next = NULL;
    return node;
  }
  Node *ptr;
  for (ptr = *head; ptr->next != NULL; ptr = ptr->next)
    ;
  ptr->next = node;
  node->next = NULL;
  return node;
}

void dealloc_list(Node **head) {
  if (*head == NULL)
    return;

  Node *cur = *head;
  Node *prev = cur;

  while (cur != NULL) {
    cur = cur->next;
    free(prev);
    prev = cur;
  }

  free(prev);
  *head = NULL;
}

void printlist(Node *head) {
  printf("/HEAD/ -> ");
  for (Node *cur = head; cur != NULL; cur = cur->next) {
    switch (cur->data.flag) {
    case int_f:
      printf("< %d >", *((int *)cur->data.data));
      break;
    case char_f:
      printf("< %c >", *((char *)cur->data.data));
      break;
    case float_f:
      printf("< %.2f >", *((float *)cur->data.data));
      break;
    case str_f:
      printf("< %s >", (char *)cur->data.data);
      break;
    case long_f:
      printf("< %ld >", *((long *)cur->data.data));
      break;
    case double_f:
      printf("< %.4lf >", *((double *)cur->data.data));
      break;
    default:
      break;
    }
    printf(" -> ");
  }
  printf("/TAIL/");
  printf("\n");
}

int list_length(Node *head) {
  if (head == NULL) {
    return 0;
  }
  int c = 0;
  for (Node *cur = head; cur != NULL; cur = cur->next) {
    c++;
  }
  return c;
}

void insert_at(Node **head, void *data, int type, int index) {
  if (list_length(*head) <= index) {
    add_end(head, data, type);
  }
  if (*head == NULL || index == 0) {
    add_beggining(head, data, type);
    return;
  }
  Node *cur = *head;
  for (int i = 0; i < index - 1; i++) {
    cur = cur->next;
  }
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return;
  }
  node->data.flag = (1 << type);
  if (fill_data(&node, data) == -1) {
    return;
  }
  node->next = cur->next;
  cur->next = node;
}

void insert_after(Node **head, void *data, int flag_s, void *data_compare) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return;
  }
  node->data.flag = (1 << flag_s);
  if (*head == NULL) {
    if (fill_data(&node, data) == -1) {
      return;
    }
    *head = node;
    return;
  }
  int found = 0;
  Node *cur;
  for (cur = *head; cur != NULL; cur = cur->next) {
    if (cur->data.flag == char_f) {
      if (*(char *)cur->data.data == *(char *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->data.flag == int_f) {
      if (*(int *)cur->data.data == *(int *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->data.flag == str_f) {
      if (strcmp((char *)cur->data.data, (char *)data_compare) == 0) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    }

    else if (cur->data.flag == float_f) {
      if (*(float *)cur->data.data == *(float *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->data.flag == long_f) {
      if (*(long *)cur->data.data == *(long *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->data.flag == double_f) {
      if (*(double *)cur->data.data == *(double *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else {
      found = 0;
      return;
    }
  }
  if (found == 0) {
    free(node);
    printf("Not found\n");
    return;
  }
  if (fill_data(&node, data) == -1) {
    free(node);
    return;
  }
  node->next = cur->next;
  cur->next = node;
  return;
}

void delete_at(Node **head, int index) {
  Node *cur = *head;
  if (*head == NULL || list_length(*head) <= index) {
    return;
  }
  if (index == 0) {
    cur = *head;
    *head = (*head)->next;
    free(cur);
    return;
  }
  for (int i = 0; i < index - 1; i++) {
    cur = cur->next;
  }
  Node *temp = cur->next;
  cur->next = temp->next;
  free(temp);
}

void delete_elem(Node **head, void *data) {
  if (*head == NULL) {
    return;
  }
  Node *cur = *head;
  int found = 0;

  if (cur->data.flag == char_f) {
    if (*(char *)cur->data.data == *(char *)data) {
      *head = cur->next;
      free(cur);
      return;
    }
  } else if (cur->data.flag == int_f) {
    if (*(int *)cur->data.data == *(int *)data) {
      *head = cur->next;
      free(cur);
      return;
    }
  } else if (cur->data.flag == str_f) {
    if (strcmp((char *)cur->data.data, (char *)data) == 0) {
      *head = cur->next;
      free(cur);
      return;
    }
  } else if (cur->data.flag == float_f) {
    if (*(float *)cur->data.data == *(float *)data) {
      *head = cur->next;
      free(cur);
      return;
    }
  } else if (cur->data.flag == long_f) {
    if (*(long *)cur->data.data == *(long *)data) {
      *head = cur->next;
      free(cur);
      return;
    }
  } else if (cur->data.flag == double_f) {
    if (*(double *)cur->data.data == *(double *)data) {
      *head = cur->next;
      free(cur);
      return;
    }
  }

  for (cur = *head; cur->next != NULL; cur = cur->next) {
    if (cur->next->data.flag == char_f) {
      if (*(char *)cur->next->data.data == *(char *)data) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->next->data.flag == int_f) {
      if (*(int *)cur->next->data.data == *(int *)data) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->next->data.flag == str_f) {
      if (strcmp((char *)cur->next->data.data, (char *)data) == 0) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->next->data.flag == float_f) {
      if (*(float *)cur->next->data.data == *(float *)data) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->next->data.flag == long_f) {
      if (*(long *)cur->next->data.data == *(long *)data) {
        found = 1;
        break;
      } else {
        found = 0;
      }

    } else if (cur->next->data.flag == double_f) {
      if (*(double *)cur->next->data.data == *(double *)data) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    }
  }

  if (found == 0) {
    printf("Element not found");
    return;
  }
  Node *temp = cur->next;
  printf("TEMP:%d\n", *(int *)temp->data.data);
  printf("CUR:%d\n", *(int *)cur->data.data);
  cur->next = temp->next;
  free(temp);
}

void insert_before(Node **head, void *data, int flag_s, void *data_compare) {
  Node *node = (Node *)malloc(sizeof(Node));
  int found = 1;
  if (node == NULL) {
    return;
  }
  node->data.flag = (1 << flag_s);
  if (*head == NULL) {
    if (fill_data(&node, data) == -1) {
      return;
    }
    *head = node;
    return;
  }

  Node *cur;
  for (cur = *head; cur->next != NULL; cur = cur->next) {
    if (cur->next->data.flag == char_f) {
      if (*(char *)cur->next->data.data == *(char *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->next->data.flag == int_f) {
      if (*(int *)cur->next->data.data == *(int *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->next->data.flag == str_f) {
      if (strcmp((char *)cur->next->data.data, (char *)data_compare) == 0) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->next->data.flag == float_f) {
      if (*(float *)cur->next->data.data == *(float *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    } else if (cur->next->data.flag == long_f) {
      if (*(long *)cur->next->data.data == *(long *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }

    } else if (cur->next->data.flag == double_f) {
      if (*(double *)cur->next->data.data == *(double *)data_compare) {
        found = 1;
        break;
      } else {
        found = 0;
      }
    }
  }
  if (found == 0) {
    free(node);
    printf("Not found\n");
    return;
  }
  if (fill_data(&node, data) == -1) {
    free(node);
    return;
  }

  node->next = cur->next;
  cur->next = node;
  return;
}
