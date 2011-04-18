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

using namespace std;

#include <iostream>

int main(int argc, char *argv[])
{
  FILE *pamfile = NULL;
  struct pam inpam;
  tuple **fontimage;
  EthernetDisplay e = EthernetDisplay(SRV_IP);

  // Read font
  printf("Reading Font... ");
  pamfile = pm_openr(argv[1]);
  fontimage = pnm_readpam(pamfile, &inpam, PAM_STRUCT_SIZE(tuple_type));
  pm_close(pamfile);
  printf("done\n");

  for (int xpos = 0; (xpos + e.getXSize() - 1) < inpam.width; xpos++) {
    // Convert from PAM to viewport
    for (int x = xpos; x < (xpos + e.getXSize()); x++) {
      for (int y = 0; y < 8; y++) {
        e.setval(x - xpos, y, 0, fontimage[y][x][0]);
        e.setval(x - xpos, y, 1, fontimage[y][x][1]);
        e.setval(x - xpos, y, 2, fontimage[y][x][2]);
      }
    }

    // Send to Arduino
    e.sync();

    // Pause between frames
    usleep(70000);
  }

  // Free up stuff
  pnm_freepamarray( fontimage, &inpam );

  return EXIT_SUCCESS;
}

