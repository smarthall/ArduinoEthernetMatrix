#define _GNU_SOURCE

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 1025
#define SRV_IP "192.168.1.15"

unsigned char buffer[96] = {
//green
0xF0, 0x00, 0x00, 0x0F,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x0F, 0xF0, 0x00,
0x00, 0x0F, 0xF0, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0xF0, 0x00, 0x00, 0x0F,
//red
0xF0, 0x00, 0x00, 0x0F,
0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0xFF, 0x00,
0x00, 0xF0, 0x0F, 0x00,
0x00, 0xF0, 0x0F, 0x00,
0x00, 0xFF, 0xFF, 0x00,
0x00, 0x00, 0x00, 0x00,
0xF0, 0x00, 0x00, 0x0F,
//blue
0xF0, 0x00, 0x00, 0x0F,
0x0F, 0xFF, 0xFF, 0xF0,
0x0F, 0x00, 0x00, 0xF0,
0x0F, 0x00, 0x00, 0xF0,
0x0F, 0x00, 0x00, 0xF0,
0x0F, 0x00, 0x00, 0xF0,
0x0F, 0xFF, 0xFF, 0xF0,
0xF0, 0x00, 0x00, 0x0F };

void diep(char *s)
{
  perror(s);
  exit(1);
}

int main(void)
{
  struct sockaddr_in si_other;
  int s, slen=sizeof(si_other);

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  if (sendto(s, buffer, sizeof(buffer), 0, (struct sockaddr *) &si_other, slen)==-1)
    diep("sendto()");

  close(s);
  return 0;
}

