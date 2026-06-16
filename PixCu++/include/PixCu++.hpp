/* PixCu++.h */
/*Includes*/
#include "Helper.hpp"
/*Includes*/

/* Typedefinations */
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long s64;
typedef float f32;
typedef double f64;
typedef char byte;
/* Typedefinations */

/* MACROS */
#define alloc(x) malloc((i64)x)
#define dealloc(x)                                                             \
  do {                                                                         \
    free(x);                                                                   \
    x = NULL;                                                                  \
  } while (0)
/* MACROS */

/* Definations */
#define ERR_STR "Error %d : %s"
/* Definations */

/* Function Signatures */
int main(int, char **);
void cpu_main(CLI &);
void gpu_main(CLI &);
/* Function Signatures */
