/* DNS_Resolver.c */
#include "DNS_Resolver.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdoslib.h>
#include <sys/socket.h>

i16 TXN_ID = 0;

void usage(char * str){  
printf("Usage %s <www.website.com>", str);
}

void panic() {
fprintf(stderr, ERR_STR, errno, strerror(errno));
exit(-1);
}

DNS * init_packet(){
DNS * ret = (DNS *)malloc(sizeof(DNS));
if (!ret) return NULL;
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
return ret;
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



i8 * change_to_dns_format(char *src) {
  i8 *dest = malloc(len(src));
  if (!dest) return NULL;
	int pos = 0,len = 0;
	int l = len(src);
	strcat(src, ".");
	for(int i = 0; i < l; ++i) {
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
  return dest;
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
 
  DNS * request = init_packet();
  char packet[65536];
  int sz = sizeof(DNS);
  copy(packet,request,sz);
  int l = len(host);
  copy(packet+sz,host,l);
  sz += l;

 int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {panic();}
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(53);
  inet_pton(AF_INET,dns_addr[1],)









  close(fd);
  return 0;
}
