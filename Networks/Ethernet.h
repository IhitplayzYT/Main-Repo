/* Ethernet.h */
#include "Networks.h"

typedef unsigned char i8;
typedef unsigned short i16;
typedef unsigned int i32;
typedef unsigned long i64;

struct s_mac{
i8 addr[6];
} packed;

typedef struct s_mac Mac;


enum e_t{unset,e_IP = 0x0800,e_ARP=0x0806}packed;
typedef enum e_t EtherType;

struct s_ether{
EtherType protocol;
Mac src,dst;
Ip* payload;
}packed;
typedef struct s_ether Ethernet;

struct s_ether_raw{
Mac dst,src;
i16 ethtype;
}packed;
typedef struct s_ether_raw Raw_Ethernet;

#define mkmac(x) _Generic((x),\
i64: to_mac,\
i8*:to_macs \
)(x)

public void show_mac(Mac *,i8);
public Mac* to_macs(i8*);
public Mac* to_mac(i64);
public i8 hex2ascii(i8*);
public i8* ascii2hex(i8);
public i8* mac2strf(Mac *);
public i8* mac2str(Mac *);
public Ethernet * init_ether(Mac*,Mac*,EtherType);
public i8* eval_ether(Ethernet *);