
#ifndef INCLUDES
#define INCLUDES
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
#endif

typedef struct Data {
  int flag;
  void *data;
} Data;

typedef struct Node {
  Data data;
  struct Node *next;
  struct Node *prev;
} Node_d;

int fill_data(Node_d **node, void *data) {
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

Node_d *add_beggining(Node_d **head, void *data, int type) {
  Node_d *node = (Node_d *)malloc(sizeof(Node_d));
  if (node == NULL) {
    return NULL;
  }
  node->data.flag = type = (1 << type);
  if (fill_data(&node, data) == -1) {
    printf("Invalid type provided\n");
    return NULL;
  }

  if (*head == NULL) {
    *head = node;
    node->next = node->prev = NULL;
    return node;
  }

  node->prev = NULL;
  node->next = *head;
  node->next->prev = node;
  *head = node;
  return node;
}

Node_d *add_end(Node_d **head, void *data, int type) {
  Node_d *node = (Node_d *)malloc(sizeof(Node_d));
  if (node == NULL) {
    return NULL;
  }
  node->data.flag = (1 << type);
  if (fill_data(&node, data) == -1) {
    printf("Invalid type provided\n");
    return NULL;
  }
  if (*head == NULL) {
    *head = node;
    node->prev = NULL;
    node->next = NULL;
    return node;
  }
  Node_d *ptr;
  for (ptr = *head; ptr->next != NULL; ptr = ptr->next)
    ;
  ptr->next = node;
  node->next = NULL;
  node->prev = ptr;
  return node;
}

void dealloc_list(Node_d **head) {
  if (*head == NULL)
    return;

  Node_d *cur = *head;

  while (cur->next != NULL) {
    cur = cur->next;
    free(cur->prev);
  }
  free(cur);
  *head = NULL;
}

void printlist(Node_d *head) {
  printf("/HEAD/ <-> ");
  for (Node_d *cur = head; cur != NULL; cur = cur->next) {
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
    printf(" <-> ");
  }
  printf("/TAIL/");
  printf("\n");
}

int list_length(Node_d *head) {
  if (head == NULL) {
    return 0;
  }
  int c = 0;
  for (Node_d *cur = head; cur != NULL; cur = cur->next) {
    c++;
  }
  return c;
}

void insert_at(Node_d **head, void *data, int type, int index) {
  if (list_length(*head) <= index) {
    add_end(head, data, type);
  }
  if (*head == NULL || index == 0) {
    add_beggining(head, data, type);
    return;
  }
  Node_d *cur = *head;
  for (int i = 0; i < index - 1; i++) {
    cur = cur->next;
  }
  Node_d *node = (Node_d *)malloc(sizeof(Node_d));
  if (node == NULL) {
    return;
  }
  node->data.flag = (1 << type);
  if (fill_data(&node, data) == -1) {
    return;
  }
  node->next = cur->next;
  node->next->prev = node;
  cur->next = node;
  node->prev = cur;
}

void insert_after(Node_d **head, void *data, int flag_s, void *data_compare) {
  Node_d *node = (Node_d *)malloc(sizeof(Node_d));
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
  Node_d *cur;
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
  node->next->prev = node;
  node->prev = cur;
  cur->next = node;
  return;
}

void delete_at(Node_d **head, int index) {
  if (*head == NULL || list_length(*head) <= index) {
    return;
  }

  Node_d *cur = *head;
  if (index == 0) {
    *head = (*head)->next;
    free((*head)->prev);
    return;
  }
  for (int i = 0; i < index - 1; i++) {
    cur = cur->next;
  }
  cur->next = cur->next->next;
  free(cur->next->prev);
  cur->next->prev = cur;
}

void delete_elem(Node_d **head, void *data) {
  if (*head == NULL) {
    return;
  }
  Node_d *cur = *head;
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
    if ((int)((*(float *)cur->data.data) * 100) ==
        (int)(*(float *)data * 100)) {
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
    if ((int)(*(double *)cur->data.data * 100) ==
        (int)(*(double *)data * 100)) {
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
      if ((int)(*(float *)cur->next->data.data * 100) ==
          (int)(*(float *)data * 100)) {
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
      if ((int)(*(double *)cur->next->data.data * 100) ==
          (int)(*(double *)data * 100)) {
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
  cur->next = cur->next->next;
  free(cur->next->prev);
  cur->next->prev = cur;
}

void insert_before(Node_d **head, void *data, int flag_s, void *data_compare) {
  Node_d *node = (Node_d *)malloc(sizeof(Node_d));
  int found = 1;
  if (node == NULL) {
    return;
  }
  node->data.flag = (1 << flag_s);
  if (*head == NULL) {
    if (fill_data(&node, data) == -1) {
      return;
    }
    node->next = node->prev = NULL;
    *head = node;
    return;
  }

  Node_d *cur;
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
  cur->next->prev = node;
  node->next = cur->next;
  node->prev = cur;
  cur->next = node;
  return;
}

void reverse_list(Node_d **head) {
  if (*head == NULL || list_length(*head) == 1) {
    return;
  }
  Node_d *cur = (*head)->next;
  Node_d *prev = *head;
  Node_d *temp;
  prev->next = NULL;
  while (cur != NULL) {
    prev->prev = cur;
    temp = cur->next;
    cur->next = prev;
    cur->prev = temp;
    prev = cur;
    cur = temp;
  }
  *head = prev;
  return;
}
