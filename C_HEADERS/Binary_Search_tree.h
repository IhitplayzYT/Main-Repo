#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct node_bst {
  int val;
  struct node_bst *left;
  struct node_bst *right;
} Node_Bst;

Node_Bst *init_tree(int x) {
  Node_Bst *root = (Node_Bst *)malloc(sizeof(Node_Bst));
  root->val = x;
  root->left = root->right = NULL;
  return root;
}

void insert(Node_Bst *root, int x) {
  if (!root) {
    return;
  }
  Node_Bst *ptr = root;
  Node_Bst *prev;
  int l = 0;
  while (ptr != NULL) {
    if (x > ptr->val) {
      prev = ptr;
      l = 0;
      ptr = ptr->right;
    } else if (x < ptr->val) {
      prev = ptr;
      l = 1;
      ptr = ptr->left;
    } else {
      return;
    }
  }
  Node_Bst *node = (Node_Bst *)malloc(sizeof(Node_Bst));
  node->val = x;
  node->left = node->right = NULL;
  if (l == 0) {
    prev->right = node;
  } else {
    prev->left = node;
  }
}
bool find(Node_Bst *root, int x) {
  if (root == NULL) {
    return false;
  }
  Node_Bst *ptr = root;
  while (ptr != NULL) {
    if (x > ptr->val) {
      ptr = ptr->right;
    } else if (x < ptr->val) {
      ptr = ptr->left;
    } else {
      return true;
    }
  }
  return false;
}

void print(Node_Bst *root) {
  if (root == NULL) {
    return;
  }
  print(root->left);
  printf("%d ", root->val);
  print(root->right);
}
