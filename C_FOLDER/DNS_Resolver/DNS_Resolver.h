/* DNS_Resolver.h */

/*Includes*/
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
#define packed __attribute__((packed))
/* MACROS */

/* Definations */
#define IP "127.0.0.1"
#define PORT 80
#define ERR_STR "Error %d : %s"
#define BUFF_SIZE 256
/* Definations */


typedef struct packed s_dns{
i16 txn_id;
i8 qr:1;
i8 opcode:4;
i8 aa:1;
i8 tc:1;
i8 rd:1;
i8 ra:1;
i8 z:1;
i8 ad:1;
i8 cd:1;
i8 rcode:4;
i16 q_no;
i16 ans_no;
i16 authority_rr;
i16 additional_authority_rr;
} DNS;

typedef struct rr_fields{
i16 type;
i16 class_;
i32 ttl;
i16 rd_len;
} RR_field;




/* Function Signatures */
int main(int, char **);
void usage(char *);
void panic();
DNS * init_packet();
char * rle_encode(char *);
char * rle_decode(char *);
/* Function Signatures */
