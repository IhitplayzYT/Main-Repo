/* Hotreload.h */

/*Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <signal.h>
/*Includes*/

/* Typedefinations */
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;
typedef float f32; 
typedef double f64;
typedef signed char s8;
typedef unsigned char byte;
/* Typedefinations */

/* MACROS */
#define public __attribute__((visibility("default")))
#define private static
#define packed __attribute__((packed))
#define internal __attribute__((visibility("hidden")))
/* MACROS */

/* Definations */
#define RELOAD(x,y) _Generic((y), \
f64 : reload_float , f32 : reload_float,\
i32 : reload_int , i64 : reload_int, int : reload_int, \
char * : reload_string ,i8 *: reload_string, s8 * : reload_string, \
byte :reload_byte, char : reload_byte , signed char :reload_byte\
)(x,y)
/* Definations */


/* Function Signatures */
int main(int, char **);
i8 update_handle(char *);
void reload_float(char *,f64);
void reload_int(char *,i64);
void reload_string(char *,char *);
void reload_byte(char *,byte);
void reload_struct(char *, void * ,size_t);
void clean_handle();
/* Function Signatures */
