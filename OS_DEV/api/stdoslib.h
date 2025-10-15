/* stdoslib.h */
#pragma once
#include <stdlib.h>

/* Typedefinations */
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;
/* Typedefinations */

/* MACROS */
#define public __attribute__((visibility("default")))
#define internal __attribute__((visibility("hidden")))
#define private static
#define fill(a,n,x) _fill((i8*)a,n,(i8)x)
#define alloc(x) malloc((int)(x))
#define dealloc(x) free((x))
#define strcopy(a,b) _copy((a),(b)) 
#define copy(a,b,n) _copyn((i8*)(a),(i8*)(b),(n),1)
#define strncopy(a,b,n) _copyn((a),(b),(n),0)
#define memcopy(a,b,n) _copyn((i8*)(a),(i8*)(b),(n),1)
#define kprintf(f,args...)printf(f"\n",args)
#define zero(a,n) _fill((i8*)a,n,0)
#define strcomp(a,b) _strcomp((i8*)a,(i8*)b)
#define compare(x,y) (_compare((x),(y)) && ((x) == (y)))
#define _compare(x,y) _Generic((x),\
signed char: _Generic((y), \
signed char:1,\
default: 0\
) \
unsigned char: _Generic((y),\
unsigned char:1,\
default: 0 \
)\
unsigned short: _Generic((y),\
unsigned short:1,\
default: 0 \
)\
signed short: _Generic((y),\
signed short:1,\
default: 0 \
)\
unsigned int: _Generic((y),\
unsigned int:1,\
default: 0 \
)\
signed int: _Generic((y),\
signed int:1,\
default: 0 \
)\
unsigned long: _Generic((y),\
unsigned long:1,\
default: 0 \
)\
signed long: _Generic((y),\
signed long:1,\
default: 0 \
)\
float: _Generic((y),\
float:1,\
default: 0 \
)\
double: _Generic((y),\
double:1,\
default: 0 \
)\
);

#define DEFINE_NUM_SORT(TYPE, NAME) \
public void NAME(TYPE *arr, i16 n, i8 asc) { \
    if(n < 2) return; \
    i16 mid = n / 2; \
    TYPE *left = alloc(mid * sizeof(TYPE)); \
    TYPE *right = alloc((n - mid) * sizeof(TYPE)); \
    for(i16 i=0;i<mid;i++) left[i]=arr[i]; \
    for(i16 i=mid;i<n;i++) right[i-mid]=arr[i]; \
    NAME(left, mid, asc); \
    NAME(right, n-mid, asc); \
    i16 i=0,j=0,k=0; \
    while(i<mid && j<n-mid) { \
        if(asc ? (left[i]<=right[j]) : (left[i]>=right[j])) arr[k++] = left[i++]; \
        else arr[k++] = right[j++]; \
    } \
    while(i<mid) arr[k++] = left[i++]; \
    while(j<n-mid) arr[k++] = right[j++]; \
    dealloc(left); dealloc(right); \
}

#ifndef IMP_DEF
#define IMP_DEF
DEFINE_NUM_SORT(char, _sort_i8)
DEFINE_NUM_SORT(short, _sort_i16)
DEFINE_NUM_SORT(int, _sort_i32)
DEFINE_NUM_SORT(long, _sort_i64)
DEFINE_NUM_SORT(unsigned char, _sort_u8)
DEFINE_NUM_SORT(unsigned short, _sort_u16)
DEFINE_NUM_SORT(unsigned int, _sort_u32)
DEFINE_NUM_SORT(unsigned long, _sort_u64)
DEFINE_NUM_SORT(float, _sort_f)
DEFINE_NUM_SORT(double, _sort_d)
#endif

#define sort_T(x,l,f) _Generic((x), \
i8 *:sort_u8 ,\
char *:sort_i8 ,\
float *sort_f ,\
double *sort_d ,\
unsigned short *: sort_u16 ,\
unsigned int*:sort_u32 ,\
unsigned long *: sort_u64 , \
signed short *: sort_i16 ,\
signed int *: sort_i32,\
signed long *: sort_i64, \
char ** : _strsort, \
default: trash\
)(x,l,f);

#define trash(...) ((void)0);
/* MACROS */
/* Function Signatures */
public void _fill (i8*,i16,i8); /* Fills fixed no of bytes to input hex/char*/
public i16 min(i16,i16);   /* Min of two numbers */
public i16 max(i16,i16);   /* Max of two numbers */
public i16 len(i8 *);      /* Length of a string */
public void _copy(i8 *,i8 *);  /* Copy contents from second to first string */
public void _copyn(i8 *,i8 *,i16,i8); /* Copy a 'N' chars from src to dest string*/
public i8* concat(i8*,i8*);
public i16 floor_div(i16,i16);
public i16 ceil_div(i16,i16);
public void print_bytes(void *, i32);
public void print_hex(void * ,i32);
public i8 _getbit(i8 *,i16);
public void _setbit(i8 *,i16);
public void _unsetbit(i8 *,i16);
public void _flipbit(i8 *,i16);
public i8 getbit(i8 *,i16);
public void _strsort(char **,i16,i8);
public void setbit(i8 *,i16);
public void unsetbit(i8 *,i16);
public void flipbit(i8 *,i16);
public double truncate(double,i8);
public signed short _strcomp(i8*,i8*);
/* Function Signatures */

