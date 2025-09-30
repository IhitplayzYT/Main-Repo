/*   main.c   */
asm(".code16gcc\n"
    ".globl _start\n"
    "_start:\n"
    "call main\n"
    "call exit\n");
#include "gui.h"
#include "assembly.h"
#include "shapes.h"
void * heapptr;

void main(){
dealloc_all();
videomode(x40_25_T);
getchar();
return;
}

void putchar(i8 c){
xputchar(c);
return;
}

void print(i8* str){
if (!str) return;
i8* p;
    for (p=str;*p;p++) putchar(*p);
return;
}

void * alloc(i16 size){
if (!size) return (void *)0;
void * p = heapptr;
heapptr += size;
return p;
}

void dealloc_all(){
    heapptr = heap;
    return;
}

i8 getchar(void){
i8 al,ah,ax,ret;
ax = xgetchar();
al = (ax & 0x0f);
ah = ((ax & 0xf0)  >> 4);
ret = (al) ? al : ah;
return ret;
}