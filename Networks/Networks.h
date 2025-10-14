/* Networks.h */
/*Includes*/
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
#define packed __attribute__((packed))
#define internal __attribute__((visibility("hidden")))
#define copy(a,b,n) _copyn((i8*)(a),(i8*)(b),(n),1)
#define strncopy(a,b,n) _copyn((i8*)(a),(i8*)(b),(n),0)
#define memcopy(a,b,n) _copyn((i8*)(a),(i8*)(b),(n),1)
/* MACROS */

/* Definations */

/* Definations */

public struct s_rawicmp{
    i8 type;
    i8 code;
    i16 checksum;
    i8 header[];
} packed;
typedef struct s_rawicmp Raw_icmp;

public enum e_type{
None,
echo,
echo_reply,
ICMP,
TCP,
UDP
}packed;

typedef enum e_type Type;

public struct s_icmp{
Type type:3;
i16 size;
i8 *header;
}packed;

typedef struct s_icmp Icmp;

public struct s_ip{
i32 srcaddr;
i32 dstaddr; 
i16 id;
Type type:3;
Icmp * payload;
}packed;

typedef struct s_ip Ip;

public struct s_raw_ip{
i8 version:4;
i8 ihl:4;
i8 dscp:6;
i8 ecn:2;
i16 len;
i16 id;
i8 flags:3;
i16 frag_offset:13;
i8 ttl;
i8 protocol;
i16 checksum;
i32 srcaddr;
i32 dstaddr;
i8 options[];
}packed;

typedef struct s_raw_ip Raw_ip;
/* Function Signatures */
int main();
public i16 endian(i16);
public i8* eval_icmp(Icmp*);
public void show_icmp(Icmp*,i8); 
public void helper_ip_icmp(Icmp*,i8); 
public void zero(i8*,i32);
public void print_hex(void *,i32);
public void _copy(i8 *,i8 *);
public void _copyn(i8 *,i8 *,i16,i8);
public i16 checksum(i8*,i16);
public i32 ipaddr(i8*);
public void show_ip(Ip*,i8);
public i8* ipstr(i32);
public i8 * eval_ip(Ip*);

/* Function Signatures */
public Icmp * init_icmp(Type,i8*,i16);
public Ip * init_ip(Type,i8*,i8*,i16);
