/* Networks.c */
#include "Networks.h"

i32 global_id;

int main() { 
srand(getpid());
global_id = rand() % 50000;
Ping * str = init_ping((i8*)"Hello\0",6,endian(5000),endian(1));
Icmp *pkt = init_icmp(echo,(i8*)str,sizeof(Ping) + 6);
Ip* ip = init_ip(ICMP,(i8*)"192.168.207.171",(i8*)"1.1.1.1",0);
if (!ip || !pkt) return -1;
ip->payload = pkt;
show(ip,1);
//print_hex(eval(ip),sizeof(Raw_icmp) + sizeof(Raw_ip) + ip->payload->size);
i32 sock = setup();
if (!sock) {close(sock);free(ip->payload);free(ip);return -1;}
int x = send_ip(sock,ip);
if (!x){close(sock);free(ip->payload);free(ip);return -1;}
Ip* reply = recv_ip(sock);
if (!reply){printf("Failed\n");return -1;}
show(reply,1);
//print_hex(eval(reply),sizeof(Raw_icmp) + sizeof(Raw_ip) + ip->payload->size);
close(sock);
free(ip->payload);
free(ip);
if (reply->payload && reply->payload->header)free(reply->payload);
if (reply->payload) free(reply);
return 0; }

public void show_ip(Ip* ip,i8 flag){
if (!ip) return;
printf("\n---IP HEADER---\nID : %d\nType : %s\n[ %s ] -> [ %s ]\n   (SRC)      ->     (DST)\n",ip->id,(ip->type == TCP)?"TCP":(ip->type == UDP)?"UDP":(ip->type == ICMP)?"ICMP":"Invalid Type",ipstr(ip->srcaddr),ipstr(ip->dstaddr));
if (ip->payload) helper_ip_icmp(ip->payload,flag);
else printf("-------------\n\n");}


public i8* eval_icmp(Icmp* icmp){
if (!icmp) return (i8*)0;
i8 * p;
Raw_icmp raw;
switch(icmp->type){
case echo:
raw.type = 8;
raw.code = 0;
break;
case echo_reply:
raw.type = 0;
raw.code = 0;
break;
case None:
default:
return (i8*)0;
break;
}
i32 size = sizeof(Raw_icmp) + icmp->size;
if (size & 1) size++;
p = (i8*)malloc(size);
size = sizeof(Raw_icmp);
memcopy(p,&raw,size);
memcopy(p+size,icmp->header,icmp->size);
i16 check = checksum(p,sizeof(Raw_icmp) + icmp->size); 
Raw_icmp * rawp = (Raw_icmp*)p;
rawp->checksum = check;
return p;
}

public i16 checksum(i8* str,i16 size){
i32 acc = 0;
i16 carry,n = size,b,sum;
i16 *p;
for (p = (i16*)str;n;p++,n-=2){
b =*p;
acc += b;
}
carry = (acc & 0xffff0000) >> 16;
sum = acc & 0x0000ffff;
return ~(sum + carry);
}


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
printf("Id: %d\n\tSeq: %d\n\t",endian(ping->id),endian(ping->seq));
}
if (df) print_hex(icmp->header,icmp->size);
printf("-------------\n\n");
}

public i8 * eval_ip(Ip* ip){
if (!ip) return (i8*)0;
Raw_ip raw;

raw.version = 4;
raw.ihl = sizeof(Raw_ip)/4;
raw.dscp = 0;
raw.ecn = 0;
raw.len = (ip->payload)? endian(sizeof(Raw_ip) + ip->payload->size + sizeof(Raw_icmp)):sizeof(Raw_ip);
raw.id = endian(ip->id);
raw.flags = 0;
raw.frag_offset= 0;
raw.ttl = 250;
raw.protocol = (ip->type == ICMP)?1:0;
raw.checksum = 0;
raw.srcaddr = ip->srcaddr;
raw.dstaddr = ip->dstaddr;

i8 * p = (i8*)malloc(raw.len);
i8*k = p;
if (!p){return (i8*)0;}

zero(p,raw.len);

memcopy(p,&raw,sizeof(Raw_ip));
p+=sizeof(Raw_ip);

if (ip->payload && ip->payload->size){
    i8* k = eval_icmp(ip->payload);
    if (k){copy(p,k,ip->payload->size+sizeof(Raw_icmp));}
    free(k);
}
Raw_ip * rawp;
rawp = (Raw_ip*)k;
rawp->checksum = endian(checksum(p,raw.len));
return k;
}

public i16 endian(i16 x){
i8 a,b;
i16 c;
a = x & 0x00ff;
b = (x & 0xff00) >> 8;
c = (a << 8) | b;
return c;
}

public int send_ip(i32 s,Ip* ip){
if (!s || !ip) return 0;
i8 * raw = eval(ip);
signed int ret;
struct sockaddr_in sock; 
zero((i8*)&sock,sizeof(sock));
sock.sin_family  = AF_INET;
sock.sin_addr.s_addr = (in_addr_t)ip->dstaddr;
i32 len = sizeof(Raw_ip);
len += (ip->payload && ip->payload->size)? sizeof(Raw_icmp) + ip->payload->size:0;
ret = sendto((int)s,raw,(int)len,0,(const struct sockaddr *)&sock,sizeof(sock));
if (ret < 0) return  0;
return 1;
}


public Ip * recv_ip(i32 s){
if (!s) return (Ip *)0;
i8 buff[MAX_PACKET_SIZE];
zero((i8*)&buff,MAX_PACKET_SIZE);
Raw_ip * raw;
Raw_icmp * rawicmp;
int n = recvfrom(s,&buff,MAX_PACKET_SIZE-1,0,0,0);
if (n < 0) return (Ip*)0;
raw = (Raw_ip*)buff;
i16 id = endian(raw->id);
i16 csum = checksum(buff,((n % 2) ? ++n : n));
if (csum){fprintf(stderr,"IP Checksum Error %.04hx! Packets Comprimised!\n",raw->checksum);return (Ip*)0;}
 
Type kind = (raw->protocol == 1) ? ICMP:None;
if (kind != ICMP){fprintf(stderr,"Unsupported Packet Type\n");return (Ip*)0;}
Ip* ip = init_ip(kind,ipstr(raw->srcaddr),ipstr(raw->dstaddr),id);
if (!ip) return (Ip *)0;
n -= sizeof(Raw_ip);
if (!n){ip->payload = (Icmp*)0;return ip;}


rawicmp = (Raw_icmp*)(buff+sizeof(Raw_ip));
Ping * ping;
Type t;
if (rawicmp->type == 8 && rawicmp->code == 0) {t = echo;}
else if (rawicmp->type == 0 && rawicmp->code == 0) {t = echo_reply;}
else {fprintf(stderr,"Unsupported Packet Type\n");return (Ip*)0;}

csum = checksum((i8*)rawicmp,(n%2)?++n:n);
if (csum){fprintf(stderr,"ICMP Checksum Error %.04hx! Packets Comprimised!\n",rawicmp->checksum);return (Ip*)0;}

n -= sizeof(Raw_icmp);
if (!n) ping = (Ping *)0;
else {ping = (Ping *)malloc(n);
if (!ping) return (Ip *)0;}
zero((i8*)ping,n);
memcopy(ping,rawicmp->header,n);
Icmp * pv = init_icmp(t,(i8*)ping,n);
if (!pv) return (Ip *)0;
ip->payload = pv;

return ip;
}

public i32 setup(){
i32 one = 1;
i32 s = socket(AF_INET,SOCK_RAW,1);
if (s <= 2) return 0;
return (setsockopt(s,SOL_IP,IP_HDRINCL,(const void *)&one,sizeof(i32)) == -1) ? 0 : s;
}




