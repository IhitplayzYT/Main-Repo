/* PROJ_NAME.h */

/*Includes*/
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
/*Includes*/

/* Typedefinations */
typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;
/* Typedefinations */

/* MACROS */
#define SWAP(X, Y) (X) ^= (Y) ^= (X) ^= (Y)
#define MALLOC(ptr, size)                                                      \
  do {                                                                         \
    ptr = malloc(size);                                                        \
    if (!ptr) {                                                                \
      fprintf(stderr, ERR_STR, errno, strerror(errno));                        \
      return NULL;                                                             \
    }                                                                          \
  } while (0);

#define public __attribute__((visibility("default")))
#define private static
/* MACROS */

/* Definations */
#define IP "127.0.0.1"
#define PORT 80
#define ERR_STR "Error %d : %s"
/* Definations */

/* Function Signatures */
int main(int, char **);
/* Function Signatures */
