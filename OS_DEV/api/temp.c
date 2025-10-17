#include "stdoslib.h"
int main() {
  int arr[] = {1, 3, 5, 6, 3, 2123, 4};
  char *strs[] = {"wdfear", "qfer", "dinjei"};
  char a = 'a', b = ']', c = 'd';
  char *x = "sadfg", *y = "qwe", *z = "aaset";
  printarr(arr, 7);
  printarr(strs, 3);
  printarr(x, y, z);
  printarr(a, b, c);

  return 0;
}
