/* stdoslib.c */
#include "stdoslib.h"
#include <stdio.h>

public void _zero(i8 *p,i16 n){
if (!p || n <= 0) return;
for (;n;p++,n--) *p = (i8)0;
}

public void _copyn(i8 *a,i8 *b,i16 n,i8 z){
if (!a || !b) return;
if (z==1) for (;n;n--,a++,b++) *a = *b;
else{
for (;n && *a != '\0' && *b != '\0';n--,a++,b++)*a = *b;
*a = '\0';
}
}

public void _copy(i8 *a,i8 *b){
if (!a || !b) return;
for (;*b != '\0';a++,b++) *a = *b;
*a = '\0';
}

public void fill (i8*a,i16 l,i8 hx){
if (!a) return;
for (;l;a++,l--) *a = hx;
}

public i16 len(i8 * str){
i16 i;
for (i = 0; str[i] != '\0';i++);
return i;
}


void print_bytes(void *ptr, i32 size) {
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



