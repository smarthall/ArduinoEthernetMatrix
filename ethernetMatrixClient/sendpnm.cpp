#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netpbm/pam.h>

#include "ethernetdisplay.h"

#define SRV_IP "192.168.1.15"

int main(int argc, char *argv[])
{
  uint8_t image[3][8][4];
  uint8_t buffer[97];
  FILE *pamfile = NULL;
  struct pam inpam;
  tuple *tuplerow;
  EthernetDisplay e = EthernetDisplay(SRV_IP);

  pamfile = pm_openr(argv[1]);
  buffer[0] = strtol(argv[2], NULL, 10);

  pnm_readpaminit(pamfile, &inpam, PAM_STRUCT_SIZE(tuple_type));

  if ( (inpam.height != e.getYSize()) || (inpam.width != e.getXSize())) {
    return EXIT_FAILURE;
  }

  tuplerow = pnm_allocpamrow(&inpam);

  for (int y = 0; y < e.getYSize(); y++) {
    pnm_readpamrow(&inpam, tuplerow);
    for (int x = 0; x < e.getXSize(); x++) {
        e.setval(x, y, 0, tuplerow[x][0]);
        e.setval(x, y, 1, tuplerow[x][1]);
        e.setval(x, y, 2, tuplerow[x][2]);
    }
  }

  pnm_freepamrow(tuplerow);

  pm_close(pamfile);

  e.sync();

  return EXIT_SUCCESS;
}

