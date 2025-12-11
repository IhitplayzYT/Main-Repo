/* Hotreload.h */

/*Includes*/
#include <ctype.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

/* Typedefinations */
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;
/* Typedefinations */

/* MACROS */
#define DEF_FNAME "./Hotreload.so"
#define public __attribute__((visibility("default")))
#define private static
#define packed __attribute__((packed))
#define internal __attribute__((visibility("hidden")))
/* MACROS */

/* Definations */

/* Definations */

/* Function Signatures */
int main(int, char **);
public
int _load_i(const char *, int, const char *);
int _load_f(const char *, float, const char *);
int _load_s(const char *, char *, const char *);

#define LOAD(x, y, z)                                                          \
  __Generic((y), int : _load_i, float : _load_f, char * : _load_s)(x, y, z)

/* Function Signatures */
