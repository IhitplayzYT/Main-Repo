/* DNS_Resolver.c */
#include "DNS_Resolver.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdoslib/stdoslib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

i16 TXN_ID = 0;

void usage(char * str){  
printf("Usage %s <www.website.com>\n", str);
}

void panic() {
fprintf(stderr, ERR_STR,__LINE__, errno, strerror(errno));
exit(-1);
}


void init_packet(char * p){
DNS *ret;
ret = (DNS *)p;
ret->txn_id = htons(TXN_ID++);
ret->flags = htons(0x0100);
ret->q_no = htons(1);
ret->ans_no = htons(0);
ret->authority_rr = htons(0);
ret->additional_authority_rr = htons(0);
}

void parse_result(char * packet){
i16 ans_no = ntohs(((DNS*)packet)->ans_no);
i16 q_no = ntohs(((DNS *)packet)->q_no);
i8 * p = (i8 *)packet + sizeof(DNS);
char name[BUFSIZ];
for (int i = 0 ; i < q_no ; ++i){
  read_name(packet,p,name);
  printf("%s:\n",name);
  p = skip_name(p);
  p += sizeof(Q_flags);
}

for (int i = 0 ;i < ans_no; i++){
parse_rr(packet,&p);
}

}


i8 *skip_name(i8 *p) {
  while (1) {
      if ((*p & 0xC0) == 0xC0) return p + 2;
      if (*p == 0) return p + 1;
      p += *p + 1;
  }
}


int read_name(char *packet, i8 *ptr, char *out) {
  int jumped = 0, len = 0;
  i8 *orig = ptr;
  while (*ptr) {
      if ((*ptr & 0xC0) == 0xC0) {
          i16 offset = ((ptr[0] & 0x3F) << 8) | ptr[1];
          ptr = (i8 *)(packet + offset);
          jumped = 1;
      } else {
          int l = *ptr++;
          for (int i = 0; i < l; i++) {
            out[len++] = *ptr++;
          }
          out[len++] = '.';
      }
  }
  if (len > 0) out[len - 1] = '\0';
  else out[0] = '\0';
  return jumped ? 2 : (ptr - orig + 1);
}



void parse_rr(char *packet, uint8_t **cursor) {
    i8 *p = *cursor;
    p = skip_name(p);
    RR_field * rr = (RR_field *)p;
    i16 type   = ntohs(rr->type);
    i16 class_ = ntohs(rr->class_);
    i32 ttl    = ntohl(rr->ttl);
    i16 rd_len  = ntohs(rr->rd_len);
    p += sizeof(RR_field);
    if (class_ != 1) {
        p += rd_len;
        *cursor = p;
        return;
    }
  if (type == 1 && rd_len == 4) {
      char ip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, p, ip, sizeof(ip));
      printf("  IPv4: %s [TTL %u]\n", ip, ttl);
  }
  else if (type == 28 && rd_len == 16) {
      char ip6[INET6_ADDRSTRLEN];
      inet_ntop(AF_INET6, p, ip6, sizeof(ip6));
      printf("  IPv6: %s [TTL %u]\n", ip6, ttl);
  }
  else if (type == 15 ){
      i16 pref;
      memcopy(&pref,p,2);
      pref = ntohs(pref);
      char mxname[256];
      read_name(packet, p + 2, mxname);
      printf("  MX: %s (Preferences: %u) [TTL %u]",mxname,pref,ttl);
  }
  else if (type == 27) {
    char longitude[64]= {0}, latitude[64] = {0}, altitude[64] = {0};
    i8 *q = p;
    q = (i8 *)read_nstr((char *)q, longitude);
    q = (i8 *)read_nstr((char *)q, latitude);
    q = (i8 *)read_nstr((char *)q, altitude);
    printf("  GPOS: Longitude=%s Latitude=%s Altitude=%s [TTL %u]\n",
           longitude, latitude, altitude, ttl);
} else if (type == 16) {  
  i8 *q = p;
  int cur_len = rd_len;
  printf("  TXT: ");
  while (cur_len > 0) {
      uint8_t len = *q++;
      cur_len--;
      if (len > cur_len) break;
      char txt[256] = {0};
      memcpy(txt, q, len);
      printf("%s", txt);
      q += len;
      cur_len -= len;
      if (cur_len > 0) {
          printf(" ");
      }
  }
  printf(" [TTL %u]\n", ttl);
} else if (type == 5) {
      char cname[256];
      read_name(packet, p, cname);
      printf("  CNAME: %s [TTL %u]\n", cname, ttl);
  }
  p += rd_len;
  *cursor = p;

}

char * read_nstr(char * p,char * out){
 uint8_t len = *p++;
 memcpy(out, p, len);
 out[len] = '\0';
 return p + len;
}


void get_dns(char * ret) {
  char line[BUFF_SIZE];
  FILE * fstream = fopen("/etc/resolv.conf", "r");
  while(fgets(line, BUFF_SIZE, fstream))
  {
    if(!strncmp(line, "nameserver", 10)) {
      if (sscanf(line, "nameserver %63s", ret) == 1) {
          ret[strcspn(ret, " \t\r\n")] = 0;
          fclose(fstream);
          return;
      }
    }
  }
  fclose(fstream);
}


int rle_encode(char *src, char *dest) {
  int pos = 0;
  int len = 0;
  int i;
  strcat(src, ".");
  for(i = 0; i < (int)len(src); ++i) {
    if(src[i] == '.') {
      dest[pos] = i - len;
      ++pos;
      for(; len < i; ++len) {
        dest[pos] = src[len];
        ++pos;
      }
      len++;
    }
  }
  dest[pos] = '\0';
  return pos;
}

char * rle_decode(char * s){
char * ret = (char *)malloc(len(s));
if (!ret) return NULL;
int l = len(s);
int k = 0;
int c_l = s[0] - '0';
for (int i = 0 ; i < l;i++){
if (s[i] - '0' <= 9 && s[i] - '0' >= 0 ){
if (c_l == 0){
  ret[k++] = '.';
  c_l = (s[i] - '0' <= 9 && s[i] - '0' >= 0 ) ? s[i] - '0' : 0;
}
else{
  c_l --;
  ret[k++] = s[i];
}
}
}
return ret;
}
void show_dns(DNS *dns) {
    i16 flags = ntohs(dns->flags);
    printf(
        "==== DNS PACKET ====\n"
        "TxnID: %u\n"
        "QR: %s\n"
        "OPCODE: %u\n"
        "FLAGS:\n"
        "  AA: %u\n"
        "  TC: %u\n"
        "  RD: %u\n"
        "  RA: %u\n"
        "  Z:  %u\n"
        "  AD: %u\n"
        "  CD: %u\n"
        "RCODE: %u\n"
        "Question Count: %u\n"
        "Answer Count: %u\n"
        "Authority RR count: %u\n"
        "Additional RR: %u\n",
        ntohs(dns->txn_id),
        DNS_QR(flags) ? "Reply" : "Query",
        DNS_OPCODE(flags),
        DNS_AA(flags),
        DNS_TC(flags),
        DNS_RD(flags),
        DNS_RA(flags),
        DNS_Z(flags),
        DNS_AD(flags),
        DNS_CD(flags),
        DNS_RCODE(flags),
        ntohs(dns->q_no),
        ntohs(dns->ans_no),
        ntohs(dns->authority_rr),
        ntohs(dns->additional_authority_rr)
    );
}


void init_q_flags(char * p,int offset){
  Q_flags * ret;
  ret = (Q_flags *)(p+offset);
  ret->qtype = htons(1);
  ret->qclass = htons(1);
}



int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage(argv[0]);
    return -1;
  }

  srand(time(0));
  char * dns_addr = (char *)malloc(INET_ADDRSTRLEN);
  if (!dns_addr) panic();
  get_dns(dns_addr);

  char packet[65536];
  zero(packet,65535);
  init_packet(packet);
  int str_len = rle_encode(argv[1],packet+sizeof(DNS))+1;
  init_q_flags(packet,sizeof(DNS)+str_len);
  int pack_len = sizeof(DNS) + sizeof(Q_flags) + str_len;


  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {free(dns_addr);panic();}
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(53);

  inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);
  sendto(fd, packet, pack_len, 0,(struct sockaddr *)&addr, sizeof(addr));
  int ret = recvfrom(fd,packet,65536,0,NULL,NULL);
  if (ret<0){
    printf("No reply\n");
  return -1;
  }
  parse_result(packet);


  close(fd);
  return 0;
}
