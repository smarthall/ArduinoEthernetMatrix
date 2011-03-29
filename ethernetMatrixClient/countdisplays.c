#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 1026
#define SRV_IP "192.168.1.15"

void diep(char *s)
{
  perror(s);
  exit(1);
}

int main(int argc, char *argv[])
{
  struct sockaddr_in si_other, si_me;
  int s, slen=sizeof(si_other);
  uint8_t data = 'C';

  // Create a socket that uses UDP datagrams
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  // Address to listen on
  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  // Machine we are sending to
  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  // Bind to the socket
  if (bind(s,(struct sockaddr*) &si_me, sizeof(si_me))==-1)
      diep("bind");

  // Send the packet
  if (sendto(s, &data, sizeof(data), 0, (struct sockaddr *) &si_other, slen)==-1)
    diep("sendto()");

  // Close the socket
  close(s);
  return 0;
}

