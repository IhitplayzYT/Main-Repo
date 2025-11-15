/* Embedder.h */

/*Includes*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*Includes*/

/* Typedefinations */
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;
/* Typedefinations */

/* MACROS */
#define public __attribute__((visibility("default")))
#define private static
#define compare(a,b) compare_((i8*)a,(i8*)b)
#define usage(x) usage_((i8*)x)
/* MACROS */

/* Definations */
#define ERR_STR "Error %d : %s"
enum e_lang {c,assm};
typedef enum e_lang Lang;
/* Definations */

/* Function Signatures */
int main(int, char **);
void usage_(i8 *);
i8 compare_(i8*,i8*);
void printheader(i8*,Lang);
void printbody(Lang);
i8* convert(i8,Lang);
i32 len(i8*);
/* Function Signatures */
