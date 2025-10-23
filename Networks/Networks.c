/* Networks.c */
#include "Networks.h"

i16 global_id;
i16 seq,id;
int main(int argc,char * argv[]) { 
srand(getpid());
global_id = rand() % 50000;
seq = 0;
i16 count = 5;
id = rand() % 50000;
i8* dst,*mssg;
i16 l;
if (argc < 2) {usage(argv[0]);return -1;}
else if (argc == 2) {
dst = (i8*)argv[1];
mssg = (i8*)"Hello";
l = 5;
}
else if (argc == 3){
dst = (i8*)argv[1];
mssg = (i8*)argv[2];
l = len(mssg);
}
else if (argc == 4){
dst = (i8*)argv[1];
mssg = (i8*)argv[2];
l = stoi((i8*)argv[3]);
}
else {usage(argv[0]);return -1;}
printf("Sending 5 packets of %d bytes ICMP echoes to %s | Timeout = %ds\n",l,dst,TIMEOUT);
i8 c = 0;
if (l > 225){printf("Mssg To Big!!\n");return -2;}
for (int i = 0 ; i < count; i++){
i8 ret = sendping(SRC_ADDR,dst,mssg,l);
if (!ret) {printf("!");c++;}
else printf(".");
fflush(stdout);
}
if (!c) {fprintf(stderr,"\nError!\n");return -3;}
printf("\nSuccess Rate (%d/%d) : %.02f %%\n",c,seq,(float)(c/5) * 100);
return 0; 
}




public i8 _sendping(i8* src,i8*dst,i8* mssg,i16 len){
if (!*src || !*dst || !*mssg) return 1;

Ping * str = init_ping((i8*)mssg,len+1,endian(id),endian(seq));
seq++;
if (!str) return 2;
Icmp *pkt = init_icmp(echo,(i8*)str,sizeof(Ping) + len+1);
if (!pkt) return 3;

Ip* ip = init_ip(ICMP,(i8*)src,(i8*)dst,0);
if (!ip) return 4;
ip->payload = pkt;
i32 sock = setup();
if (!sock) {close(sock);free(ip->payload);free(ip);return 5;}
int x = send_ip(sock,ip);
if (!x){close(sock);free(ip->payload);free(ip);return 6;}
//
Ip* reply = recv_ip(sock);
//
if (!reply){
    close(sock);free(ip->payload);free(ip);free(reply);return 7;}
close(sock);
free(ip->payload);
free(ip);
if (reply->payload && reply->payload->header)free(reply->payload);
if (reply->payload) free(reply);
return 0;
}


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
i16 t = sizeof(Raw_icmp);
memcopy(p,&raw,t);
memcopy(p+t,icmp->header,icmp->size);
i16 check = checksum(p,size); 
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

public i8 * eval_ip(Ip* ip){
if (!ip) return (i8*)0;
Raw_ip raw;
raw.version = 4;
raw.ihl = (sizeof(Raw_ip))/4;
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

public i32 setup(){
i32 one = 1;
i32 s = socket(AF_INET,SOCK_RAW,1);
if (s <= 2) return 0;
struct timeval t;
t.tv_sec = TIMEOUT;
t.tv_usec = 0;
if (setsockopt(s,SOL_IP,IP_HDRINCL,(const void *)&one,sizeof(i32)) == -1) return 0;
 if (setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,&t,sizeof(t)) == -1) {close(s);return 0;}
  return s;
}
