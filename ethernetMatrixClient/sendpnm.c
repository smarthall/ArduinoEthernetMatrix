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
#include <netpbm/pam.h>

#define PORT 1025
#define SRV_IP "192.168.1.15"

void diep(char *s)
{
  perror(s);
  exit(1);
}

void twodots(uint8_t *value, int num1, int num2) {
    num1 /= 16;
    num2 /= 16;
    *value = (num1 * 16) + num2;
}

int main(int argc, char *argv[])
{
  struct sockaddr_in si_other;
  int s, slen=sizeof(si_other);
  uint8_t image[3][8][4];
  FILE *pamfile = NULL;
  struct pam inpam;
  tuple *tuplerow;
  unsigned int row;

  pamfile = pm_openr(argv[1]);

  pnm_readpaminit(pamfile, &inpam, PAM_STRUCT_SIZE(tuple_type));

  if ( (inpam.height != 8) || (inpam.width != 8)) {
    exit(1);
  }

  tuplerow = pnm_allocpamrow(&inpam);

  for (row = 0; row < 8; row++) {
    unsigned int column;
    pnm_readpamrow(&inpam, tuplerow);
    for (column = 0; column < 8; column += 2) {
      // Green
      twodots( image[0][row] + ( column / 2 ),
               tuplerow[column][1],
               tuplerow[column + 1][1]);
      // Red
      twodots( image[1][row] + ( column / 2 ),
               tuplerow[column][0],
               tuplerow[column + 1][0]);
      // Blue
      twodots( image[2][row] + ( column / 2 ),
               tuplerow[column][2],
               tuplerow[column + 1][2]);
    }
  }

  pnm_freepamrow(tuplerow);

  pm_close(pamfile);

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  if (sendto(s, image, sizeof(image), 0, (struct sockaddr *) &si_other, slen)==-1)
    diep("sendto()");

  close(s);
  return 0;
}

