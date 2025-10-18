#include "stdoslib.h"

int main() {
  int y = 72;
  Vector *v = Vector_init(&y, 32);
  int x = 45;
  v->append(v, &x);
  int z = 32;
  v->append(v, &z);
  v->pop(v);
  printf("%d\n", *(int *)v->data[0]);
  printf("%d\n", *(int *)v->data[1]);
  printf("%d\n", *(int *)v->data[2]);
}
