/*   main.c   */
asm(".code16gcc\n"
    ".globl _start\n"
    "_start:\n"
    "call main\n"
    "call exit\n");
#include "gui.h"
#include "assembly.h"
#include "shapes.h"
#include "bmp.h"

void * heapptr,* heapsaved;

void main(){
videoflag = 0;
dealloc_all();
heapptr = heap1;
i16 thickness = 5;
i8 color = 2;
Point * p1 = init_point(1,1,color);
Point * p2 = init_point(400,400,color);
Rectangle *r = init_rectangle(p1,p2,color,0,1,thickness);
if (!r) return;
videomode(x640_480_16);
draw_rectangle(r);
getchar();
dealloc_all();
return;
}

void putchar(i8 c){
xputchar(c);
return;
}

void print_x(i8* str){
if (!str) return;
i8* p;
    for (p=str;*p;p++) putchar(*p);
return;
}

void * alloc(i16 size){
if (!size) return (void *)0;
i16 n_size = heapptr - heap1 + size;
if (n_size > heapsize) return (void *)0;
void * p = heapptr;
heapptr = (void *)((i8 *)heapptr + size);
return p;
}

void dealloc_all(){
    heapptr = heap1;
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

i8* tostr(i16 x){
static i8 buff[20];
i8 i = 18;
if (x == 0) buff[i--] = '0';
else{
while (x > 0){
buff[i--] = '0' + x % 10;
x = x/10;
}}
buff[19] = '\0';
return (buff+i+1);
}


void save(){
    heapsaved = heapptr;
}

void load(){
    heapptr = heapsaved;
}