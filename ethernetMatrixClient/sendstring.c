#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __APPLE__
  #include <pam.h>
#else
  #include <netpbm/pam.h>
#endif

#include "viewport.h"

void bitblit(tuple ** dest, int destx, int desty, tuple **src, int srcx, int srcy, int width, int height) {
  int maxx = srcx + width;
  int maxy = srcy + height;

  int xdiff = destx - srcx;
  int ydiff = desty - srcy;

  for (int x = srcx; x < maxx; x++) {
    for (int y = srcy; y < maxy; y++) {
      //fprintf(stderr, "(%d, %d) -> (%d, %d)\n", x, y, x + xdiff, y + ydiff);
      memcpy(dest[y + ydiff][x + xdiff], src[y][x], sizeof(sample) * 3);
    }
  }
}

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
  for (int x = 0; x < mempam.width; x++)
    for (int y = 0; y < mempam.height; y++)
      memimage[y][x] = pnm_allocpamtuple(&mempam);
  
  for (int i = 0; i < textlen; i++) {
    char ch = argv[2][i];
    bitblit(memimage, i * 8, 0, fontimage, (ch - 32) * 8, 0, 8, 8);
  }

  int senddisplay = strtol(argv[3], NULL, 10);

  // Make a viewport
  display = allocviewport(senddisplay);

  for (int xpos = 0; (xpos + 7) < mempam.width; xpos++) {
    // Convert from PAM to viewport
    for (int x = xpos; x < (xpos+8); x++) {
      for (int y = 0; y < 8; y++) {
        setval(display, x - xpos, y, 0, memimage[y][x][0]);
        setval(display, x - xpos, y, 1, memimage[y][x][1]);
        setval(display, x - xpos, y, 2, memimage[y][x][2]);
      }   
    }   

    // Send to Arduino
    sendimage(display, "192.168.1.15", 1025);

    // Pause between frames
    usleep(70000);
  }

  // Free up stuff
  for (int x = 0; x < mempam.width; x++)
    for (int y = 0; y < mempam.height; y++)
      pnm_freepamtuple(memimage[y][x]);
  freeviewport(display);
  pnm_freepamarray( fontimage, &inpam );
  pnm_freepamarray( memimage, &mempam );

  return 0;
}

