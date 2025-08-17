#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void insertion_sort(int *arr, int m, int asc) {
  if (asc) {
    for (int i = 1; i < m; i++) {
      int t = arr[i];
      int j = i - 1;
      while (j >= 0 && arr[j] > t) {
        arr[j + 1] = arr[j];
        j--;
      }
      arr[j + 1] = t;
    }
  } else {
    for (int i = 1; i < m; i++) {
      int t = arr[i];
      int j = i - 1;
      while (j >= 0 && arr[j] < t) {
        arr[j + 1] = arr[j];
        j--;
      }
      arr[j + 1] = t;
    }
  }
}

void print_array(int *arr, int m) {
  for (int i = 0; i < m; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void print_array2d(int **arr, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", arr[i][j]);
    }
    printf("\n");
  }
}

void read_array(int *arr, int m) {
  for (int i = 0; i < m; i++) {
    scanf("%d", &arr[i]);
  }
}

void read_array2d(int **arr, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%d", &arr[i][j]);
    }
  }
}

void binary_sort(int *arr, int m, int asc) {
  if (asc) {
    for (int i = 0; i < m - 1; i++) {
      for (int j = 0; j < m - i - 1; j++) {
        if (arr[j] > arr[j + 1]) {
          arr[j] ^= arr[j + 1] ^= arr[j] ^= arr[j + 1];
        }
      }
    }
  } else {
    for (int i = 0; i < m - 1; i++) {
      for (int j = 0; j < m - i - 1; j++) {
        if (arr[j] > arr[j + 1]) {
          arr[j] ^= arr[j + 1] ^= arr[j] ^= arr[j + 1];
        }
      }
    }
  }
}

void selection_sort(int *arr, int m, int asc) {
  if (asc) {
    for (int i = 0; i < m; i++) {
      int min = i;
      for (int j = i + 1; j < m; j++) {
        if (arr[j] < arr[min])
          min = j;
      }
      int temp = arr[i];
      arr[i] = arr[min];
      arr[min] = temp;
    }
  } else {
    for (int i = 0; i < m; i++) {
      int min = i;
      for (int j = i + 1; j < m; j++) {
        if (arr[j] > arr[min])
          min = j;
      }
      int temp = arr[i];
      arr[i] = arr[min];
      arr[min] = temp;
    }
  }
}

void merge(int *larr, int *rarr, int *arr, int m, int asc) {
  int j = 0;
  int l = (int)(m / 2);
  int L, R;
  L = R = 0;
  if (asc == 1) {
    while (L < l && R < m - l) {
      if (larr[L] <= rarr[R])
        arr[j++] = larr[L++];
      else
        arr[j++] = rarr[R++];
    }
  } else {
    while (L < l && R < m - l) {
      if (larr[L] >= rarr[R])
        arr[j++] = larr[L++];
      else
        arr[j++] = rarr[R++];
    }
  }

  while (L < l)
    arr[j++] = larr[L++];

  while (R < m - l)
    arr[j++] = rarr[R++];
}

void merge_sort(int *arr, int m, int asc) {
  if (m <= 1)
    return;
  int l = (int)(m / 2);
  int *t1 = (int *)malloc(sizeof(int) * l);
  int *t2 = (int *)malloc(sizeof(int) * (m - l));
  memcpy(t1, arr, l * sizeof(int));
  memcpy(t2, arr + l, (m - l) * sizeof(int));
  merge_sort(t1, l, asc);
  merge_sort(t2, m - l, asc);
  merge(t1, t2, arr, m, asc);
}

int binary_search(int *arr, int m, int to_find) {
  int L = 0;
  int R = m - 1;
  while (L <= R) {
    int mid = L + (R - L) / 2;
    if (arr[mid] > to_find) {
      R = mid - 1;
    } else if (arr[mid] < to_find) {
      L = mid + 1;
    } else {
      return mid;
    }
  }

  return -1;
}
