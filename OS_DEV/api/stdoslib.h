/* stdoslib.h */
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
/* MACROS */

/* Definations */
#define IP "127.0.0.1"
#define PORT 80
#define ERR_STR "Error %d : %s\n"
/* Definations */

/* Function Signatures */
public void fill (i8*,i32,i8); /* Fills fixed no of bytes to input hex/char*/
public void zero(i8 *,i8); /* Zeroes out all the bytes */
public int min(int,int);  /* Min of two numbers */
public int max(int,int);   /* Max of two numbers */
public int len(i8 *);   /* Length of a string */
public void copy(i8 *,i8 *);  /* Copy contents from second to first string */
public void copyn(i8 *,i8 *,i8); /* Copy a 'N' chars from src to dest string*/
/* Function Signatures */
