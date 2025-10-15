/* stdoslib.c */
#include "stdoslib.h"
#include <stdio.h>


public void _copyn(i8 *a,i8 *b,i16 n,i8 z){
if (!a || !b) return;
if (z==1) for (;n;n--,a++,b++) *a = *b;
else{
for (;n && *a != '\0' && *b != '\0';n--,a++,b++)*a = *b;
*a = '\0';
}
}

public signed short _strcomp(i8* a,i8* b){
i8 * p = a,*q = b;
while (*p && *q){
if (*p > *q) return 1;
if (*p < *q) return -1;
p++;q++;}
return (*p == *q)?0:(!*p)?-1:1;
}

public void print_hex(void * str,i32 n){
i32 i = 0,j;
for (;i < n ;i ++){
printf("0x%02hhX ",*((i8*)str+i));
if (!((i+1) % 16)){
for (i16 m = i - 15;m <= i;m++) printf("%c", (*((i8*)str+m) >=32 && *((i8*)str+m) <= 126)? *((i8*)str+m) : '.' );
printf("\n");}}
int start = n - (n % 16);
if (n % 16) {
    printf("\t");
    for (int k = 0 ; k < start/8;k++) printf("    ");
    for (j = start; j < n; j++) {printf("%c", ((*((i8*)str+j) >= 32 && *((i8*)str+j) <= 126) ? *((i8*)str+j) : '.'));}
} 
printf("\n");
}


public void _copy(i8 *a,i8 *b){
if (!a || !b) return;
for (;*b != '\0';a++,b++) *a = *b;
*a = '\0';
}

public void _fill (i8*a,i16 l,i8 hx){
if (!a) return;
for (;l;a++,l--) *a = hx;
}

public i16 len(i8 * str){
i16 i;
for (i = 0; str[i] != '\0';i++);
return i;
}


public void print_bytes(void *ptr, i32 size) {
    if (!ptr)return;
    i8 *p = ptr;
    for (i32 i = 0; i < size; i++) {
        printf("%02X ", p[i]);   
        if ((i + 1) % 16 == 0)  
            printf("\n");
    }
    printf("\n");
}

public i16 min(i16 a,i16 b){return (a >= b) ? b : a;}
public i16 max(i16 a,i16 b){return (a >= b) ? a : b;}

public i16 ceil_div(i16 a,i16 b){
return (!a % b) ? (i16)a/b : ((i16)a/b) + 1;
}
public i16 floor_div(i16 a,i16 b){
return (i16)a/b;
}

public i8 _getbit(i8 * a,i16 n){
return (((1 << n) & *a) >> n );
}
public void _setbit(i8 * a,i16 n){
*a |= (1 << n);
}
public void _unsetbit(i8 * a,i16 n){
*a &= (~(1 << n)) ;
}
public void _flipbit(i8 *a,i16 n){
*a ^= (1 << n) ;
}


public i8* concat(i8*a,i8*b){
static i8 buff[512];
strcopy(buff,a);
strcopy(&buff[len(a)],b);
buff[len(buff)] = '\0';
return buff;
}

public i8 getbit(i8 *str,i16 idx){
i16 bytes_move;
i8 bits_move;
bytes_move = (i16)idx/8;
bits_move = idx % 8;
void * mem = (void *)(str+bytes_move);
i8 bit = _getbit((i8*)mem,bits_move);
return bit;
}

public void setbit(i8 *str,i16 idx){
i16 bytes_move;
i8 bits_move;
bytes_move = (i16)idx/8;
bits_move = idx % 8;
void * mem = (void *)(str+bytes_move);
_setbit((i8*)mem,bits_move);
}

public void unsetbit(i8 *str,i16 idx){
i16 bytes_move;
i8 bits_move;
bytes_move = (i16)idx/8;
bits_move = idx % 8;
void * mem = (void *)(str+bytes_move);
_unsetbit((i8*)mem,bits_move);
}

public void flipbit(i8 *str,i16 idx){
i16 bytes_move;
i8 bits_move;
bytes_move = (i16)idx/8;
bits_move = idx % 8;
void * mem = (void *)(str+bytes_move);
_flipbit((i8*)mem,bits_move);
}

public double truncate(double x,i8 n){
    i64 t = 1;
    for (int i = 0;i< n;i++) t*= 10;
    i64 l = x * t;
    x = (double)l / t;
return x;
}

void _strsort(char **arr, i16 n, i8 asc) {
    if(n < 2) return;
    i16 mid = n / 2;
    char **left = malloc(mid * sizeof(char*));
    char **right = malloc((n-mid) * sizeof(char*));
    for(i16 i=0;i<mid;i++) left[i]=arr[i];
    for(i16 i=mid;i<n;i++) right[i-mid]=arr[i];

    _strsort(left, mid, asc);
    _strsort(right, n-mid, asc);

    i16 i=0,j=0,k=0;
    while(i<mid && j<n-mid) {
        if(asc ? (strcomp(left[i], right[j]) <= 0) : (strcomp(left[i], right[j]) >= 0))
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }
    while(i<mid) arr[k++] = left[i++];
    while(j<n-mid) arr[k++] = right[j++];
    free(left); free(right);
}
