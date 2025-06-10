/* Server.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/stat.h>

#define IP "127.0.0.1"

typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;

struct httpReq
{
    char method[8];
    char url[128];
};
typedef struct httpReq Req;
void http_header(int, char *, int);
void http_response(int, char *, char *);
Req *parsehttp(char *);
int serv_init(int);
int client_accept(int);
void client_connection(int, int);
char *client_read(int);
int main(int, char **);