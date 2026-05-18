#pragma once
/* VigiFile.h */
/*Includes*/



#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
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
#define true 1
#define True 1
#define false 0
#define False 0
#define public __attribute__((visibility("default")))
#define private static
#define internal __attribute__((visibility = "hidden"))
#define packed __attribute__((packed))
#define FREE(...) freeall(__VA_ARGS__,NULL)
#define MAX_LEN 100
#define MAX_WORD_LEN 32

/* MACROS */

typedef struct s_CLI{
char excludes[MAX_LEN][MAX_WORD_LEN];
char includes[MAX_LEN][MAX_WORD_LEN];
}CLI;


typedef enum e_ECODES{
  ESUCC,
  EARGS,
  EINIT,
  EDEINIT
} ECODE;

/* Definations */
#define ERR_STR "Error %d : %s"
#define zero(x,y) _zero((void *)x,y)
/* Definations */

/* Function Signatures */
int main(int, char **);
int init(int*,int*,CLI*);
int deinit();
void freeall(void * x,...);
CLI* arg_parse(int argc,char ** argv);
void _zero(void *,int);
/* Function Signatures */

//    struct inotify_event {
//       int      wd;       /* Watch descriptor */
//       uint32_t mask;     /* Mask describing event */
//       uint32_t cookie;   /* Unique cookie associating related
//                             events (for rename(2)) */
//       uint32_t len;      /* Size of name field */
//       char     name[];   /* Optional null-terminated name */
//    };
