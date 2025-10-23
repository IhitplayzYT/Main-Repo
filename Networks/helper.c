#include "Networks.h"

public void _copyn(i8 *a,i8 *b,i16 n,i8 z){
if (!a || !b) return;
if (z==1) for (;n;n--,a++,b++) *a = *b;
else{
for (;n && *a != '\0' && *b != '\0';n--,a++,b++)*a = *b;
*a = '\0';
}
}

public void zero(i8* str,i32 n){
i8* p;
for (p =str;n;n--,p++) *p = 0;
}

void print_hex(void *str, i32 n) {
i32 i, j;
i8 *data = (i8*)str;
for (i = 0; i < n; i += 16) {
for (j = 0; j < 16; j++) {
    if (i + j < n) printf("0x%02hhX ", data[i + j]);
    else printf("     "); }
printf(" ");
for (j = 0; j < 16 && i + j < n; j++) {
    i8 c = data[i + j];
    printf("%c", (c >= 32 && c <= 126) ? c : '.');}
printf("\n");
}}

public void show_icmp(Icmp* icmp,i8 df){
if (!icmp){return;}
printf("\n---ICMP HEADER---\nType : %s\nSize : %d Bytes\n",(icmp->type == None)?"Default":(icmp->type == echo)?"Echo":(icmp->type == echo_reply)?"Echo Reply":"Invalid Type",icmp->size);
if (icmp->type == echo || icmp->type == echo_reply){
Ping * ping = (Ping *)icmp->header;
printf("Id: %d\nSeq: %d\n",endian(ping->id),endian(ping->seq));
}
if (df) print_hex(icmp->header,icmp->size);
printf("-------------\n\n");
}

public void helper_ip_icmp(Icmp* icmp,i8 df){
if (!icmp){return;}
printf("\n  [Payload]\n\tType : %s\n\tSize : %d Bytes\n\t",(icmp->type == None)?"Default":(icmp->type == echo)?"Echo":(icmp->type == echo_reply)?"Echo Reply":"Invalid Type",icmp->size);
if (icmp->type == echo || icmp->type == echo_reply){
Ping * ping = (Ping *)icmp->header;
printf("Id: %d\n\tSeq: %d\n",endian(ping->id),endian(ping->seq));
}
printf("%d\n ",icmp->size-sizeof(Ping));
if (df) print_hex(icmp->header+4,icmp->size-sizeof(Ping)-1);
printf("-------------\n\n");
}

public void show_ip(Ip* ip,i8 flag){
if (!ip) return;
printf("\n---IP HEADER---\nID : %d\nType : %s\n[ %s ] -> [ %s ]\n   (SRC)      ->     (DST)\n",ip->id,(ip->type == TCP)?"TCP":(ip->type == UDP)?"UDP":(ip->type == ICMP)?"ICMP":"Invalid Type",ipstr(ip->srcaddr),ipstr(ip->dstaddr));
if (ip->payload) helper_ip_icmp((Icmp*)ip->payload,flag);
else printf("-------------\n\n");}

public i32 ipaddr(i8* s){
i8 a[4] = {0},*p,c = 0;
i32 ret;
for (p = s;*p;p++){
if (*p == '.' || *p == '-' || *p == ':') c++;
else{
a[c] *= 10;
a[c] += *p - '0';
}}
ret = (a[3] << 24) | (a[2] << 16) | (a[1] << 8) | a[0];
return ret;
}

public i8* ipstr(i32 addr){
i8 *buff = (i8*)malloc(16);
zero(buff,16);
i8 a[4];
a[0] = (addr & 0xff000000) >> 24;
a[1] = (addr & 0x00ff0000) >> 16;
a[2] = (addr & 0x0000ff00) >> 8;
a[3] = (addr & 0x000000ff);
snprintf((char*)buff, 16, "%u.%u.%u.%u", a[3], a[2], a[1], a[0]);
return buff;
}

public i16 endian(i16 x){
i8 a,b;
i16 c;
a = x & 0x00ff;
b = (x & 0xff00) >> 8;
c = (a << 8) | b;
return c;
}

