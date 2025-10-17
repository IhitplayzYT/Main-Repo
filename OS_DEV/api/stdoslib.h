/* stdoslib.h */
#pragma once
#include <stdio.h>
#include <stdlib.h>

/* Typedefinations */
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long s64;
typedef float f32;
typedef double f64;
/* Typedefinations */

/* MACROS */
#define public __attribute__((visibility("default")))
#define internal __attribute__((visibility("hidden")))
#define private static
#define fill(a, n, x) _fill((i8 *)a, n, (i8)x)
#define alloc(x) malloc((int)(x))
#define dealloc(x) free((x))
#define strcopy(a, b) _copy((a), (b))
#define copy(a, b, n) _copyn((i8 *)(a), (i8 *)(b), (n), 1)
#define strncopy(a, b, n) _copyn((a), (b), (n), 0)
#define memcopy(a, b, n) _copyn((i8 *)(a), (i8 *)(b), (n), 1)
#define kprintf(f, args...) printf(f "\n", args)
#define zero(a, n) _fill((i8 *)a, n, 0)
#define strcomp(a, b) _strcomp((i8 *)(a), (i8 *)(b))
static signed short _strcomp(i8 *, i8 *);

#define compare(x, y) _compare((x), (y)) && ((x) == (y))
#define _compare(x, y)                                                         \
  _Generic((x),                                                                \
      signed char: _Generic((y),                                               \
          signed char: 1,                                                      \
          default: 0) unsigned char: _Generic((y),                             \
          unsigned char: 1,                                                    \
          default: 0) unsigned short: _Generic((y),                            \
          unsigned short: 1,                                                   \
          default: 0) signed short: _Generic((y),                              \
          signed short: 1,                                                     \
          default: 0) unsigned int: _Generic((y),                              \
          unsigned int: 1,                                                     \
          default: 0) signed int: _Generic((y),                                \
          signed int: 1,                                                       \
          default: 0) unsigned long: _Generic((y),                             \
          unsigned long: 1,                                                    \
          default: 0) signed long: _Generic((y),                               \
          signed long: 1,                                                      \
          default: 0) float: _Generic((y),                                     \
          float: 1,                                                            \
          default: 0) double: _Generic((y), double: 1, default: 0));

#define DEFINE_NUM_SORT(TYPE, NAME)                                            \
  static inline void NAME(TYPE *arr, i16 n, i8 asc) {                          \
    if (n < 2)                                                                 \
      return;                                                                  \
    i16 mid = n / 2;                                                           \
    TYPE *left = alloc(mid * sizeof(TYPE));                                    \
    TYPE *right = alloc((n - mid) * sizeof(TYPE));                             \
    for (i16 i = 0; i < mid; i++)                                              \
      left[i] = arr[i];                                                        \
    for (i16 i = mid; i < n; i++)                                              \
      right[i - mid] = arr[i];                                                 \
    NAME(left, mid, asc);                                                      \
    NAME(right, n - mid, asc);                                                 \
    i16 i = 0, j = 0, k = 0;                                                   \
    while (i < mid && j < n - mid) {                                           \
      if (!asc ? (left[i] <= right[j]) : (left[i] >= right[j]))                \
        arr[k++] = left[i++];                                                  \
      else                                                                     \
        arr[k++] = right[j++];                                                 \
    }                                                                          \
    while (i < mid)                                                            \
      arr[k++] = left[i++];                                                    \
    while (j < n - mid)                                                        \
      arr[k++] = right[j++];                                                   \
    dealloc(left);                                                             \
    dealloc(right);                                                            \
  }

#define DEFINE_STRING_SORT(TYPE,NAME) \
static inline void NAME(TYPE **arr, i16 n, i8 asc) {\
    if(n < 2) return;\
    i16 mid = n / 2;\
    TYPE **left = alloc(mid * sizeof(TYPE*));\
    TYPE **right = alloc((n-mid) * sizeof(TYPE*));\
    for(i16 i=0;i<mid;i++) left[i]=arr[i];\
    for(i16 i=mid;i<n;i++) right[i-mid]=arr[i];\
    NAME(left, mid, asc);\
    NAME(right, n-mid, asc);\
    i16 i=0,j=0,k=0;\
    while(i<mid && j<n-mid) {\
        if(!asc ? (strcomp(left[i], right[j]) <= 0) : (strcomp(left[i], right[j]) >= 0))\
            arr[k++] = left[i++];\
        else\
            arr[k++] = right[j++];\
    }\
    while(i<mid) arr[k++] = left[i++];\
    while(j<n-mid) arr[k++] = right[j++];\
    dealloc(left); dealloc(right);\
}

#define DEFINE_MIN_STR(TYPE,name) \
static inline TYPE* name(TYPE* X,...){\
va_list args; \
va_start(args, X); \
TYPE* next;\
TYPE* min = X;\
while ((next = va_arg(args,TYPE*))){\
  if (strcomp(next,min) == -1) min = next;\
}\
return min;\
}

#define DEFINE_MAX_STR(TYPE,name) \
static inline TYPE* name(TYPE* X,...){\
va_list args; \
va_start(args, X); \
TYPE* next;\
TYPE* max = X;\
while ((next = va_arg(args,TYPE*))){\
  if (strcomp(next,max) == 1) max = next;\
}\
return max; \
}

#define DEFINE_MIN(TYPE,name) \
static inline TYPE name(TYPE X,...){\
va_list args; \
va_start(args, X); \
TYPE next;\
TYPE min = X;\
while ((next = va_arg(args,TYPE))){\
  if (min > next) min = next;\
}\
return min;\
}

#define DEFINE_MAX(TYPE,name) \
static inline TYPE name(TYPE X,...){\
va_list args; \
va_start(args, X); \
TYPE next;\
TYPE max = X;\
while ((next = va_arg(args,TYPE))){\
  if (max < next) max = next;\
}\
return max; \
}

#define DEFINE_MIN_ARR(TYPE,name) \
static inline TYPE name(TYPE* X,...){\
TYPE min = *X; \
va_list args; \
va_start(args,X);\
i32 l =  va_arg(args,i32);\
for (int i = 0; i < l ;i ++) if (*(X+i) < min) min = *(X+i);\
return min; \
}

#define DEFINE_MAX_ARR(TYPE,name) \
static inline TYPE name(TYPE* X,...){\
TYPE max = *X; \
va_list args; \
va_start(args,X);\
i32 l =  va_arg(args,i32);\
for (int i = 0; i < l ;i ++) if (*(X+i) > max) max = *(X+i);\
return max; \
}

#define DEFINE_MIN_ARR_S(TYPE,name) \
static inline TYPE* name(TYPE** X,...){\
TYPE* min = *X; \
va_list args; \
va_start(args,X);\
i32 l =  va_arg(args,i32);\
for (int i = 0; i < l ;i ++) if (strcomp(min,*(X+i))==1) min = *(X+i);\
return min; \
}

#define DEFINE_MAX_ARR_S(TYPE,name) \
static inline TYPE* name(TYPE** X,...){\
TYPE* max = *X; \
va_list args; \
va_start(args,X);\
i32 l =  va_arg(args,i32);\
for (int i = 0; i < l ;i ++) if (strcomp(max,*(X+i))==-1) max = *(X+i);\
return max; \
}

typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

#define trash(arg,...) ((void)0)

#ifndef IMP_DEF
#define IMP_DEF
DEFINE_NUM_SORT(char, sort_i8)
DEFINE_NUM_SORT(short, sort_i16)
DEFINE_NUM_SORT(int, sort_i32)
DEFINE_NUM_SORT(long, sort_i64)
DEFINE_NUM_SORT(unsigned char, sort_u8)
DEFINE_NUM_SORT(unsigned short, sort_u16)
DEFINE_NUM_SORT(unsigned int, sort_u32)
DEFINE_NUM_SORT(unsigned long, sort_u64)
DEFINE_NUM_SORT(float, sort_f)
DEFINE_NUM_SORT(double, sort_d)

DEFINE_MIN_ARR(i16,min_i16a);
DEFINE_MIN_ARR(i32,min_i32a);
DEFINE_MIN_ARR(i64,min_i64a);
DEFINE_MIN_ARR(s16,min_s16a);
DEFINE_MIN_ARR(s32,min_s32a);
DEFINE_MIN_ARR(s64,min_s64a);
DEFINE_MIN_ARR(f32,min_fa);
DEFINE_MIN_ARR(f64,min_da);
DEFINE_MIN_ARR_S(i8,min_i8sa);
DEFINE_MIN_ARR_S(s8,min_s8sa);
DEFINE_MIN_ARR_S(char,min_csa);


DEFINE_MAX_ARR(i16,max_i16a);
DEFINE_MAX_ARR(i32,max_i32a);
DEFINE_MAX_ARR(i64,max_i64a);
DEFINE_MAX_ARR(s16,max_s16a);
DEFINE_MAX_ARR(s32,max_s32a);
DEFINE_MAX_ARR(s64,max_s64a);
DEFINE_MAX_ARR(f32,max_fa);
DEFINE_MAX_ARR(f64,max_da);
DEFINE_MAX_ARR_S(i8,max_i8sa);
DEFINE_MAX_ARR_S(s8,max_s8sa);
DEFINE_MAX_ARR_S(char,max_csa);

DEFINE_MIN(i8,min_i8);
DEFINE_MIN(i16,min_i16);
DEFINE_MIN(i32,min_i32);
DEFINE_MIN(i64,min_i64);
DEFINE_MIN(s8,min_s8);
DEFINE_MIN(s16,min_s16);
DEFINE_MIN(s32,min_s32);
DEFINE_MIN(s64,min_s64);
DEFINE_MIN(f32,min_f);
DEFINE_MIN(f64,min_d);
DEFINE_MIN_STR(i8,min_i8s);
DEFINE_MIN_STR(s8,min_s8s);
DEFINE_MAX(i8,max_i8);
DEFINE_MAX(i16,max_i16);
DEFINE_MAX(i32,max_i32);
DEFINE_MAX(i64,max_i64);
DEFINE_MAX(s8,max_s8);
DEFINE_MAX(s16,max_s16);
DEFINE_MAX(s32,max_s32);
DEFINE_MAX(s64,max_s64);
DEFINE_MAX(f32,max_f);
DEFINE_MAX(f64,max_d);
DEFINE_MAX_STR(i8,max_i8s);
DEFINE_MAX_STR(s8,max_s8s);


static signed short _strcomp(i8* a,i8* b){
i8 * p = a,*q = b;
while (*p && *q){
if (*p > *q) return 1;
if (*p < *q) return -1;
p++;q++;}
return (*p == *q)?0:(!*p)?-1:1;
}
DEFINE_STRING_SORT(char,_strsort)
#endif

#define max(x,...) _Generic((x),\
i8:max_i8, \
i16:max_i16, \
i32:max_i32, \
i64:max_i64, \
s8:max_s8, \
s16:max_s16, \
s32:max_s32, \
s64:max_s64, \
f32:max_f, \
f64:max_d, \
i8*:max_i8s,\
s8*:max_s8s,\
i16*:max_i16a, \
i32*:max_i32a, \
i64*:max_i64a, \
s16*:max_s16a, \
s32*:max_s32a, \
s64*:max_s64a, \
f32*:max_fa, \
f64*:max_da, \
i8**:max_i8sa,\
s8**:max_s8sa,\
char**:max_csa \
)(x,__VA_ARGS__,NULL)

#define min(x,...) _Generic((x),\
i8:min_i8, \
i16:min_i16, \
i32:min_i32, \
i64:min_i64, \
s8:min_s8, \
s16:min_s16, \
s32:min_s32, \
s64:min_s64, \
f32:min_f, \
f64:min_d, \
i8*:min_i8s,\
s8*:min_s8s,\
i16*:min_i16a, \
i32*:min_i32a, \
i64*:min_i64a, \
s16*:min_s16a, \
s32*:min_s32a, \
s64*:min_s64a, \
f32*:min_fa, \
f64*:min_da, \
i8**:min_i8sa,\
s8**:min_s8sa,\
char**:min_csa \
)(x,__VA_ARGS__,NULL)


#define sort(x, l, f)                                                          \
  _Generic((x),                                                                \
      i8 *: sort_u8,                                                           \
      char *: sort_i8,                                                         \
      float *: sort_f,                                                         \
      double *: sort_d,                                                        \
      unsigned short *: sort_u16,                                              \
      unsigned int *: sort_u32,                                                \
      unsigned long *: sort_u64,                                               \
      signed short *: sort_i16,                                                \
      signed int *: sort_i32,                                                  \
      signed long *: sort_i64,                                                 \
      char **: _strsort                                                        \
) (x, l, f);

#define endian(x) _Generic((x),\
i16:endian16,\
i32:endian32,\
i64:endian64\
)(x)


/* MACROS */

/* Function Signatures */
public void _fill(i8 *, i16, i8); /* Fills fixed no of bytes to input hex/char*/
public i16 len(i8 *); /* Length of a string */
public void _copy(i8 *, i8 *); /* Copy contents from second to first string */
public void _copyn(i8 *, i8 *, i16, i8); /* Copy a 'N' chars from src to dest string*/
public i8 *concat(i8 *, i8 *);
public i16 floor_div(i16, i16);
public i16 ceil_div(i16, i16);
public void print_bytes(void *, i32);
public void print_hex(void *, i32);
public i8 _getbit(i8 *, i16);
public void _setbit(i8 *, i16);
public void _unsetbit(i8 *, i16);
public void _flipbit(i8 *, i16);
public i8 getbit(i8 *, i16);
public void setbit(i8 *, i16);
public void unsetbit(i8 *, i16);
public void flipbit(i8 *, i16);
public double truncate(double, i8);
public i32 ipaddr(i8*);
public i8* ipstr(i32);
public i16 net_port(i16);
public i16 endian16(i16 x);
public i32 endian32(i32 x);
public i64 endian64(i64 x);
/* Function Signatures */
