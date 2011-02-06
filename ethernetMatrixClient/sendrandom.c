#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 96
#define NPACK 1
#define PORT 1025
#define SRV_IP "192.168.1.15"

FILE *random = NULL;

void diep(char *s)
{
  perror(s);
  exit(1);
}

int main(void)
{
  struct sockaddr_in si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFLEN];

  random = fopen("/dev/urandom", "r");

  if (random == NULL) {
      exit(1);
  }

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  for (i=0; i<NPACK; i++) {
    printf("Sending packet %d\n", i);
    fread(buf, sizeof(char), 96, random);
    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, slen)==-1)
      diep("sendto()");
  }

  fclose(random);
  close(s);
  return 0;
}

