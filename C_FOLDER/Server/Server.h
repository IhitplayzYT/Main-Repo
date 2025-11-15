/* Server.h */
#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define IP "127.0.0.1"

typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;

struct httpReq {
  char method[8];
  char url[128];
};
typedef struct httpReq Req;
void http_header(int, char *, int);
void http_response(int, char *, char *);
void parsehttp(char *, Req *);
int serv_init(int);
int client_accept(int);
void client_connection(int, int);
char *client_read(int);
int main(int, char **);
void read_file(char *, char *, int mode);
