#include "stdoslib.h"

int main() {
  int y = 72;
  Vector *v = Vector_init(&y, 32);
  int x = 45;
  v->append(v, &x);
  int z = 32;
  v->append(v, &z);
  Iterator *iter = v->iterator(v);
  void *d = iter->next(iter);
  d = iter->next(iter);
  printf("%d ", *(int *)d);
}
