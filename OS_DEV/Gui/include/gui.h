/*  gui.h  */
#pragma once
/*Includes*/
/*Includes*/

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
#define packed __attribute__((packed))
#define print(x) print_x((i8*)x)
/* MACROS */

/* Definations */
#define IP "127.0.0.1"
#define PORT 80
#define ERR_STR "Error %d : %s\n"
/* Definations */

extern void *heap1;
extern i16 heapsize;

/* Function Signatures */
void print_x(i8*);
void putchar(i8);
i8 getchar();
i8 * tostr(i16);
void save();
void load();
void *alloc(i16);
void dealloc_all();
void videomode(i8);
/* Function Signatures */


