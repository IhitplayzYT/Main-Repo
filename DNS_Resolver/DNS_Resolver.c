/* DNS_Resolver.c */
#include "DNS_Resolver.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdoslib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

i16 TXN_ID = 0;

void usage(char * str){  
printf("Usage %s <www.website.com>", str);
}

void panic() {
fprintf(stderr, ERR_STR, errno, strerror(errno));
exit(-1);
}

void init_packet(char * p){
DNS *ret;
ret = (DNS *)p;
ret->txn_id = TXN_ID++;
ret->qr = 0;
ret->opcode = 0;
ret->aa = 0;
ret->tc = 0;
ret->rd = 1;
ret->ra = 0;
ret->z = 0;
ret->ad = 0;
ret->cd=1;
ret->rcode =0;
ret->q_no = htons(1);
ret->ans_no = 0;
ret->authority_rr = 0;
ret->additional_authority_rr = 0;
}

DNS * parse_response(){




}

void get_dns(char ** ret) {
  FILE *fstream;
  char line[BUFF_SIZE];
  int i = 0;
  fstream = fopen("/etc/resolv.conf", "rt");
  while(fgets(line, BUFF_SIZE, fstream))
  {
    if(!strncmp(line, "nameserver", 10)) {
      strcopy(ret[i], strtok(line, " "));
      strcopy(ret[i], strtok(NULL, "\n"));
      ++i;
    }
  }
  fclose(fstream);
}


char * rle_decode(char * s){
char * ret = (char *)malloc(len(s)+1);
if (!ret) return NULL;
char buff[2] = {0};
struct s_Tok_ret * token = tokenise(s,'.');
for (int i = 0 ;i < token->n;i++)
{snprintf(buff,2,"%d",len(token->ret[i]));
concat((i8 *)ret,(i8 *)buff);
concat((i8 *)ret, token->ret[i++]);
}
return ret;
}

char * rle_encode(char * s){
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


void show_dns(DNS * d){

}




int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage(argv[0]);
    return -1;
  }
  char * host = rle_encode(argv[1]);
  srand(time(0));
  char **dns_addr = malloc(5 * sizeof(char *));
  if (!dns_addr) return -1;
    for(int i = 0; i < 5; ++i) 
    {dns_addr[i] = malloc(INET_ADDRSTRLEN);
    if (!dns_addr[i]) {free(dns_addr);return -1;}}

  get_dns(dns_addr);
  char packet[65536];
  init_packet(packet);
  int sz = sizeof(DNS);
  copy(&packet + sz,argv[1],len(argv[1]));
  sz += len(argv[1])+1;
  // QFLAGS
  
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {free(host);free(dns_addr);panic();}

  struct sockaddr_in addr;
  zero(&addr,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(53);
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(fd,(struct sockaddr * )&addr, sizeof(addr)) == -1){
  free(host);
  free(dns_addr);
  close(fd);
  panic();
  }

  write(fd,packet,sz);




    
  close(fd);
  return 0;
}
