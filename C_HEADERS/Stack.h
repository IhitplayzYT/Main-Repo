
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct data {
  void *arr;
  int *flag;
} Data;

typedef struct stack {
  Data data;
  int size;
  int capacity;
} Stack;

#define int_f 0b000001    // 0
#define char_f 0b000010   // 1
#define float_f 0b000100  // 2
#define str_f 0b001000    // 3
#define long_f 0b010000   // 4
#define double_f 0b100000 // 5

Stack *init_stack(int x) {
  if (x <= 0)
    return NULL;
  Stack *start = (Stack *)malloc(sizeof(Stack));
  start->capacity = x;
  start->size = 0;
  start->data.arr = (void *)malloc(sizeof(void *) * x);
  start->data.flag = (int *)malloc(sizeof(int) * x);
  memset(start->data.flag, 0, sizeof(int) * x);
  memset(start->data.arr, 0, sizeof(void *) * x);
  return start;
}

int is_empty(Stack *start) { return (start->size == 0) ? 1 : 0; }
int is_full(Stack *start) { return (start->size == start->capacity) ? 1 : 0; }

void deallocate(Stack **start) {
  free((*start)->data.arr);
  free((*start)->data.flag);
  free(*start);
  *start = NULL;
  return;
}

void push(Stack *start, void *data, int flag) {
  if (is_full(start)) {
    printf("Stack full\n");
    return;
  }
  if (flag == 0) {
    ((int *)(start->data.arr))[start->size] = *((int *)data);
  } else if (flag == 1) {
    ((char *)(start->data.arr))[start->size] = *((char *)data);
  } else if (flag == 2) {
    ((float *)(start->data.arr))[start->size] = *((float *)data);
  } else if (flag == 3) {
    ((char **)(start->data.arr))[start->size] = ((char *)data);
  } else if (flag == 4) {
    ((long *)(start->data.arr))[start->size] = *((long *)data);
  } else if (flag == 5) {
    ((double *)(start->data.arr))[start->size] = *((double *)data);
  } else {
    return;
  }
  start->data.flag[start->size] = (1 << flag);
  start->size++;

  return;
}

void pop(Stack *start) {
  if (is_empty(start)) {
    return;
  }
  start->size--;
  return;
}

void printStack(Stack *start) {

  for (int i = start->size - 1; i >= 0; i--) {
    if (start->data.flag[i] == int_f) {
      printf("\t%d\n", ((int *)(start->data.arr))[i]);
    } else if (start->data.flag[i] == char_f) {
      printf("\t%c\n", ((char *)(start->data.arr))[i]);
    } else if (start->data.flag[i] == float_f) {
      printf("\t%10.2f\n", ((float *)(start->data.arr))[i]);
    } else if (start->data.flag[i] == str_f) {
      printf("\t%s\n", ((char **)(start->data.arr))[i]);
    } else if (start->data.flag[i] == long_f) {
      printf("\t%10ld\n", ((long *)(start->data.arr))[i]);
    } else if (start->data.flag[i] == double_f) {
      printf("\t%10.4lf\n", ((double *)(start->data.arr))[i]);
    } else {
      return;
    }

    printf(" |\n");
  }

  printf(" START ");
  printf("----------------------------\n");
}

void prettyprintstack(Stack *start) {

  for (int i = start->size - 1; i >= 0; i--) {
    if (start->data.flag[i] == int_f) {
      printf("╭--------------------╮\n|       %d        "
             "|\n╰--------------------╯\n",
             ((int *)(start->data.arr))[i]);
    } else if (start->data.flag[i] == char_f) {

      printf("╭--------------------╮\n|         %c          "
             "|\n╰--------------------╯\n",
             ((char *)(start->data.arr))[i]);
    } else if (start->data.flag[i] == float_f) {
      printf("╭--------------------╮\n|    %10.2f     "
             " |\n╰--------------------╯\n",
             ((float *)(start->data.arr))[i]);
    } else if (start->data.flag[i] == str_f) {

      printf("╭--------------------╮\n|       %s        "
             "|\n╰--------------------╯\n",
             ((char **)(start->data.arr))[i]);
    } else if (start->data.flag[i] == long_f) {

      printf("╭--------------------╮\n|     %10ld     "
             "|\n╰--------------------╯\n",
             ((long *)(start->data.arr))[i]);

    } else if (start->data.flag[i] == double_f) {

      printf("╭--------------------╮\n|     %10.4lf    "
             " |\n╰--------------------╯\n",
             ((double *)(start->data.arr))[i]);
    } else {
      return;
    }

    printf("           |\n");
  }

  printf("╭--------------------╮\n|        START      "
         " |\n╰--------------------╯\n\n");

  printf("----------------------------\n");
}

void emptyStack(Stack *start) {
  start->size = 0;
  return;
}

void expand_stack(Stack **start) {
  if (is_full(*start)) {
    Stack *temp;
    temp->data.arr = realloc((*start)->data.arr, (*start)->capacity * 2);
    (*start)->data.arr = temp->data.arr;
  } else {
    printf("Stack still not full !\n");
  }
}
