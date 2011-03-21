#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netpbm/pam.h>

#include "viewport.h"

int main(int argc, char *argv[])
{
  FILE *pamfile = NULL;
  struct pam inpam;
  tuple **fontimage;
  viewport display;

  // Read font
  printf("Reading Font... ");
  pamfile = pm_openr(argv[1]);
  fontimage = pnm_readpam(pamfile, &inpam, PAM_STRUCT_SIZE(tuple_type));
  pm_close(pamfile);
  printf("done\n");

  // Make a viewport
  display = allocviewport();

  // Convert from PAM to viewport
  for (int x = 8; x < 16; x++) {
    for (int y = 0; y < 8; y++) {
      setval(display, x - 8, y, 0, fontimage[y][x][0]);
      setval(display, x - 8, y, 0, fontimage[y][x][1]);
      setval(display, x - 8, y, 0, fontimage[y][x][2]);
    }
  }

  // Send to Arduino
  sendimage(display, "192.168.1.15", 1025);

  // Free up stuff
  freeviewport(display);
  pnm_freepamarray( fontimage, &inpam );

  return 0;
}

