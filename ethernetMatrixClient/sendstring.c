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
  struct pam inpam, mempam;
  tuple **fontimage, **memimage;
  viewport display;
  int textlen = strlen(argv[2]);

  // Read font
  pamfile = pm_openr(argv[1]);
  fontimage = pnm_readpam(pamfile, &inpam, PAM_STRUCT_SIZE(tuple_type));
  pm_close(pamfile);

  // Setup In-memory Pam file
  mempam.size = sizeof(mempam);
  mempam.len = PAM_STRUCT_SIZE(tuple_type);
  mempam.file = stdout;
  mempam.format = PPM_FORMAT;
  mempam.plainformat = 0;
  mempam.height = 8;
  mempam.width = textlen * 8;
  mempam.depth = 3;
  mempam.maxval = 255;
  strcpy(mempam.tuple_type, PAM_PPM_TUPLETYPE);
  memimage = pnm_allocpamarray(&mempam);

  // Make a viewport
  display = allocviewport();

  // Convert from PAM to viewport
  //for (int x = xpos; x < (xpos+8); x++) {
  //  for (int y = 0; y < 8; y++) {
  //    setval(display, x - xpos, y, 0, fontimage[y][x][0]);
  //    setval(display, x - xpos, y, 1, fontimage[y][x][1]);
  //    setval(display, x - xpos, y, 2, fontimage[y][x][2]);
  //  }
  //}

  // Send to Arduino
  //sendimage(display, "192.168.1.15", 1025);

  // Free up stuff
  freeviewport(display);
  pnm_freepamarray( fontimage, &inpam );
  pnm_freepamarray( memimage, &mempam );

  return 0;
}

