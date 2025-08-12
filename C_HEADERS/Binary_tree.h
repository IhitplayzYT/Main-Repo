
#ifndef BINTREE
#define BINTREE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define int_f 0b000001    // 0
#define char_f 0b000010   // 1
#define float_f 0b000100  // 2
#define str_f 0b001000    // 3
#define long_f 0b010000   // 4
#define double_f 0b100000 // 5
#define Epsilon 0.001
#endif

typedef struct node_t {
  void *data;
  int flag;
  struct node_t *left;
  struct node_t *right;
} Node_t;

Node_t *init_node(void *data, int flag) {
  Node_t *root = (Node_t *)malloc(sizeof(Node_t));
  root->flag = 1 << flag;
  root->left = root->right = NULL;
  root->data = data;
  return root;
}

void add_left(Node_t *root, void *data, int flag) {
  Node_t *temp = root;
  while (temp->left != NULL) {
    temp = temp->left;
  }
  Node_t *node = init_node(data, flag);
  temp->left = node;
}

void add_right(Node_t *root, void *data, int flag) {
  Node_t *temp = root;
  while (temp->right != NULL) {
    temp = temp->right;
  }
  Node_t *node = init_node(data, flag);
  temp->right = node;
}

int is_empty(Node_t *root) { return root == NULL ? 1 : 0; }

void addchild(Node_t **root, void *data_to_find, void *data_to_insert, int flag,
              int *found) {
  if (*found == 1) {
    return;
  }

  if (*root == NULL) {
    printf("Please init tree");
    return;
  }
  Node_t *temp = *root;
  if (temp->flag == int_f && *((int *)temp->data) == *((int *)data_to_find)) {
    if (temp->left == NULL) {
      temp->left = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else if (temp->right == NULL) {
      temp->right = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else {
      printf("Found node already has two children\n");
      *found = 1;
      return;
    }

  } else if (temp->flag == char_f &&
             *(char *)temp->data == *(char *)data_to_find) {
    if (temp->left == NULL) {
      temp->left = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else if (temp->right == NULL) {
      temp->right = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else {
      printf("Found node already has two children\n");
      *found = 1;
      return;
    }

  } else if (temp->flag == float_f &&
             *(float *)temp->data == *(float *)data_to_find) {
    if (temp->left == NULL) {
      temp->left = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else if (temp->right == NULL) {
      temp->right = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else {
      printf("Found node already has two children\n");
      *found = 1;
      return;
    }

  } else if (temp->flag == str_f &&
             (char *)temp->data == (char *)data_to_find) {
    if (temp->left == NULL) {
      temp->left = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else if (temp->right == NULL) {
      temp->right = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else {
      printf("Found node already has two children\n");
      *found = 1;
      return;
    }

  } else if (temp->flag == long_f &&
             *(long *)temp->data == *(long *)data_to_find) {
    if (temp->left == NULL) {
      temp->left = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else if (temp->right == NULL) {
      temp->right = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else {
      printf("Found node already has two children\n");
      *found = 1;
      return;
    }

  } else if (temp->flag == double_f &&
             *(double *)temp->data == *(double *)data_to_find) {
    if (temp->left == NULL) {
      temp->left = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else if (temp->right == NULL) {
      temp->right = init_node(data_to_insert, flag);
      *found = 1;
      return;
    } else {
      printf("Found node already has two children\n");
      *found = 1;
      return;
    }

  } else {
    addchild(&(temp->left), data_to_find, data_to_insert, flag, found);
    addchild(&(temp->right), data_to_find, data_to_insert, flag, found);
  }

  return;
}

void free_subtree(Node_t **node_ptr) {
  if (node_ptr == NULL || *node_ptr == NULL)
    return;

  Node_t *node = *node_ptr;
  free_subtree(&(node->left));
  free_subtree(&(node->right));
  free(node);
  *node_ptr = NULL;
}

void deletechild(Node_t **root, void *data_to_find, int *found) {
  if (*found == 1 || *root == NULL) {
    return;
  }
  Node_t *t = *root;
  if (t->flag == int_f) {
    if (t->left && *(int *)t->left->data == *(int *)data_to_find) {
      *found = 1;
      free_subtree(&t->left);
      return;
    } else if (t->right && *(int *)t->right->data == *(int *)data_to_find) {
      *found = 1;
      free_subtree(&t->right);
      return;
    }
  } else if (t->flag == char_f) {

    if (t->left && *(char *)t->left->data == *(char *)data_to_find) {
      *found = 1;
      free_subtree(&t->left);
      return;

    } else if (t->right && *(char *)t->right->data == *(char *)data_to_find) {
      *found = 1;
      free_subtree(&t->right);
      return;
    }

  } else if (t->flag == float_f) {
    if (fabs(*(float *)t->left->data - *(float *)data_to_find) < Epsilon) {
      *found = 1;
      free_subtree(&t->left);
      return;
    } else if (fabs(*(float *)t->right->data - *(float *)data_to_find) <
               Epsilon) {
      *found = 1;
      free_subtree(&t->right);
      return;
    }
  } else if (t->flag == str_f) {
    if (t->left && strcmp((char *)t->left->data, (char *)data_to_find) == 0) {
      *found = 1;
      free_subtree(&t->left);
      return;
    } else if (t->right &&
               strcmp((char *)t->right->data, (char *)data_to_find) == 0) {
      *found = 1;
      free_subtree(&t->right);
      return;
    }
  } else if (t->flag == long_f) {
    if (t->left && *(long *)t->left->data == *(long *)data_to_find) {
      *found = 1;
      free_subtree(&t->left);
      return;
    } else if (t->right && *(long *)t->right->data == *(long *)data_to_find) {
      *found = 1;
      free_subtree(&t->right);
      return;
    }

  } else if (t->flag == double_f) {
    if (fabs(*(double *)t->left->data - *(double *)data_to_find) < Epsilon) {
      *found = 1;
      free_subtree(&t->left);
      return;
    } else if (fabs(*(double *)t->right->data - *(double *)data_to_find) <
               Epsilon) {
      *found = 1;
      free_subtree(&t->right);
      return;
    }
  }
  if (*found == 0) {
    deletechild(&(t->left), data_to_find, found);
    deletechild(&(t->right), data_to_find, found);
  }
}

void print_tree(Node_t *root, int method) {
  if (root == NULL) {
    return;
  }

  if (root->flag == int_f) {
    printf("%d ", *(int *)root->data);
  } else if (root->flag == char_f) {
    printf("%c ", *(char *)root->data);
  } else if (root->flag == float_f) {
    printf("%f ", *(float *)root->data);
  } else if (root->flag == str_f) {
    printf("%s ", (char *)root->data);
  } else if (root->flag == long_f) {
    printf("%ld ", *(long *)root->data);
  } else if (root->flag == double_f) {
    printf("%lf ", *(double *)root->data);
  } else {
  }

  if (root->left != NULL) {
    print_tree(root->left, method);
  }

  if (root->right != NULL) {
    print_tree(root->right, method);
  }
  return;
}

void prettyprint_tree(Node_t *root, const char *prefix, int isLeft) {
  if (root == NULL)
    return;

  printf("%s", prefix);
  printf(isLeft ? "├── " : "└── ");

  if (root->flag == int_f) {
    printf("%d\n ", *(int *)root->data);
  } else if (root->flag == char_f) {
    printf("%c\n ", *(char *)root->data);
  } else if (root->flag == float_f) {
    printf("%.2f\n ", *(float *)root->data);
  } else if (root->flag == str_f) {
    printf("%s\n ", (char *)root->data);
  } else if (root->flag == long_f) {
    printf("%.4ld\n ", *(long *)root->data);
  } else if (root->flag == double_f) {
    printf("%lf\n ", *(double *)root->data);
  } else {
  }

  char newPrefix[256];
  strcpy(newPrefix, prefix);
  strcat(newPrefix, isLeft ? "│   " : "    ");

  if (root->left || root->right) {
    if (root->left)
      prettyprint_tree(root->left, newPrefix, 1);
    if (root->right)
      prettyprint_tree(root->right, newPrefix, 0);
  }
}

void inorder(Node_t *root) {
  if (root) {
    inorder(root->left);
    if (root->flag == int_f) {
      printf("%d ", *(int *)root->data);
    } else if (root->flag == char_f) {
      printf("%c ", *(char *)root->data);
    } else if (root->flag == float_f) {
      printf("%.2f ", *(float *)root->data);
    } else if (root->flag == str_f) {
      printf("%s ", (char *)root->data);
    } else if (root->flag == long_f) {
      printf("%.4ld ", *(long *)root->data);
    } else if (root->flag == double_f) {
      printf("%lf ", *(double *)root->data);
    } else {
    }
    inorder(root->right);
  }
}
void preorder(Node_t *root) {
  if (root) {
    if (root->flag == int_f) {
      printf("%d ", *(int *)root->data);
    } else if (root->flag == char_f) {
      printf("%c ", *(char *)root->data);
    } else if (root->flag == float_f) {
      printf("%.2f ", *(float *)root->data);
    } else if (root->flag == str_f) {
      printf("%s ", (char *)root->data);
    } else if (root->flag == long_f) {
      printf("%.4ld ", *(long *)root->data);
    } else if (root->flag == double_f) {
      printf("%lf ", *(double *)root->data);
    } else {
    }
    preorder(root->left);
    preorder(root->right);
  }
}
void postorder(Node_t *root) {
  if (root) {
    postorder(root->left);
    postorder(root->right);
    if (root->flag == int_f) {
      printf("%d ", *(int *)root->data);
    } else if (root->flag == char_f) {
      printf("%c ", *(char *)root->data);
    } else if (root->flag == float_f) {
      printf("%.2f ", *(float *)root->data);
    } else if (root->flag == str_f) {
      printf("%s ", (char *)root->data);
    } else if (root->flag == long_f) {
      printf("%.4ld ", *(long *)root->data);
    } else if (root->flag == double_f) {
      printf("%lf ", *(double *)root->data);
    } else {
    }
  }
}

void dealloc_tree(Node_t **root) {
  free_subtree(root);
  *root = NULL;
}

int get_height(Node_t *node) {
  if (node == NULL) {
    return -1;
  }
  int left_height = get_height(node->left);
  int right_height = get_height(node->right);
  return 1 + (left_height > right_height ? left_height : right_height);
}

int get_size(Node_t *root) {
  if (root == NULL)
    return 0;
  return 1 + get_size(root->left) + get_size(root->right);
}
