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
/* MACROS */

/* Definations */
#define IP "127.0.0.1"
#define PORT 80
#define ERR_STR "Error %d : %s\n"
/* Definations */

extern void *heap;

/* Function Signatures */
void print(i8*);
void putchar(i8);
i8 getchar();
void *alloc(i16);
void dealloc_all();
void videomode(i8);
/* Function Signatures */


