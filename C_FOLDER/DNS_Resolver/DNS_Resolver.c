/* DNS_Resolver.c */
#include "DNS_Resolver.h"
#include <netdb.h>

typedef struct ret_struc {
  char Ip[13];
  int port;
} Return;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage %s <www.website.com>", argv[0]);
    return -1;
  }
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    fprintf(stderr, ERR_STR, errno, strerror(errno));
    return -2;
  }

  struct addrinfo *addr, *hints;
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_CANONNAME;

  if (getaddrinfo(argv[1], "53", hints, &addr) != 0) {
    close(fd);
    freeaddrinfo(addr);
    return -3;
  }

  freeaddrinfo(addr);
  close(fd);
  return 0;
}
