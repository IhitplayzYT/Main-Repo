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
#define ERR_STR "[%d]Error %d : %s"
#define BUFF_SIZE 256
#define DNS_QR(f)      (((f) >> 15) & 0x1)
#define DNS_OPCODE(f) (((f) >> 11) & 0xF)
#define DNS_AA(f)      (((f) >> 10) & 0x1)
#define DNS_TC(f)      (((f) >> 9)  & 0x1)
#define DNS_RD(f)      (((f) >> 8)  & 0x1)
#define DNS_RA(f)      (((f) >> 7)  & 0x1)
#define DNS_Z(f)       (((f) >> 6)  & 0x1)
#define DNS_AD(f)      (((f) >> 5)  & 0x1)
#define DNS_CD(f)      (((f) >> 4)  & 0x1)
#define DNS_RCODE(f)   ((f) & 0xF)

/* Definations */


typedef struct s_dns{
i16 txn_id;
i16 flags;
i16 q_no;
i16 ans_no;
i16 authority_rr;
i16 additional_authority_rr;
} packed DNS;

typedef struct rr_fields{
i16 type;
i16 class_;
i32 ttl;
i16 rd_len;
}packed RR_field;


typedef struct s_q{
i16 qtype;
i16 qclass;
}packed Q_flags;


/* Function Signatures */
int main(int, char **);
void usage(char *);
void panic();
void init_packet(char *);
int rle_encode(char *,char *);
char * rle_decode(char *);
void parse_result(char *);
void get_dns(char *);
void show_dns(DNS *);
void init_q_flags(char *,int);
i8 * skip_name(i8 *);
void parse_rr(char *,i8 **);
int read_name(char *,i8 *,char *);
char * read_nstr(char *,char *);
/* Function Signatures */
