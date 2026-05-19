#pragma once
/* VigiFile.h */
/*Includes*/

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <signal.h>
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
#define MAX_PATH_LEN 1028
#define MAX_FDS 256

#define INITIAL_CAPACITY 128
/* MACROS */

typedef struct s_CLI{
char excludes[MAX_LEN][MAX_WORD_LEN];
char includes[MAX_LEN][MAX_WORD_LEN];
}CLI;

typedef struct s_Queue
{
    char **data;
    size_t front;
    size_t back;
    size_t capacity;
} Queue;


typedef enum e_ECODES{
  ESUCC,
  EARGS,
  EINIT,
  EMEM,
  EDEINIT
} ECODE;

/* Definations */
#define ERR_STR "Error %d : %s"
#define zero(x,y) _zero((i8 *)x,y)
#define BITMASK IN_ACCESS |IN_ATTRIB |IN_CLOSE_WRITE |IN_CREATE |IN_DELETE |IN_DELETE_SELF |IN_MODIFY | IN_CLOSE_NOWRITE | IN_OPEN | IN_CREATE | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO | IN_IGNORED | IN_Q_OVERFLOW | IN_UNMOUNT | IN_ISDIR
/* Definations */

/* Function Signatures */
int main(int, char **);
int init(int*,char x[MAX_LEN][MAX_WORD_LEN],CLI *);
int deinit(int*,char x[MAX_LEN][MAX_WORD_LEN],CLI *);
CLI* arg_parse(int argc,char ** argv);
void _zero(i8 *,int);

void recurse_add(int * ,char x[MAX_LEN][MAX_WORD_LEN],int*);
/* Function Signatures */

//    struct inotify_event {
//       int      wd;       /* Watch descriptor */
//       uint32_t mask;     /* Mask describing event */
//       uint32_t cookie;   /* Unique cookie associating related
//                             events (for rename(2)) */
//       uint32_t len;      /* Size of name field */
//       char     name[];   /* Optional null-terminated name */
//    };

void queue_init(Queue *q);
void queue_destroy(Queue *q);
char *dequeue(Queue *q);
int queue_empty(Queue *q);
void enqueue(Queue *q, const char *path);
void sigint_handler(int signo);
void parse_event(struct inotify_event * events);