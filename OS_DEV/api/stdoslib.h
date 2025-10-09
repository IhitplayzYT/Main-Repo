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
#define alloc(x) malloc((int)(x))
#define dealloc(x) free((x))
#define strcopy(a,b) _copy((a),(b)) 
#define zero(a,b) _zero((i8*)a,b)
#define strncopy(a,b,n) _copyn((a),(b),(n),0)
#define memcopy(a,b,n) _copyn((i8*)(a),(i8*)(b),(n),1)
#define kprintf(f,args...)printf(f"\n",args)
/* MACROS */

/* Definations */
#define IP "127.0.0.1"
#define PORT 80
#define ERR_STR "Error %d : %s\n"
/* Definations */

/* Function Signatures */
public void fill (i8*,i16,i8); /* Fills fixed no of bytes to input hex/char*/
public void _zero(i8 *,i16); /* Zeroes out all the bytes */
public i16 min(i16,i16);  /* Min of two numbers */
public i16 max(i16,i16);   /* Max of two numbers */
public i16 len(i8 *);   /* Length of a string */
public void _copy(i8 *,i8 *);  /* Copy contents from second to first string */
public void _copyn(i8 *,i8 *,i16,i8); /* Copy a 'N' chars from src to dest string*/
public i8* concat(i8*,i8*);
public i16 floor_div(i16,i16);
public i16 ceil_div(i16,i16);
void print_bytes(void *, i32);
public i8 _getbit(i8 *,i16);
public void _setbit(i8 *,i16);
public void _unsetbit(i8 *,i16);
public void _flipbit(i8 *,i16);
public i8 getbit(i8 *,i16);
public void setbit(i8 *,i16);
public void unsetbit(i8 *,i16);
public void flipbit(i8 *,i16);
/* Function Signatures */
