#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netpbm/pam.h>

void sendimage(uint8_t *image);

int main(int argc, char *argv[])
{
  FILE *pamfile = NULL;
  struct pam inpam;
  tuple **fontimage;

  // Read font
  printf("Reading Font... ");
  pamfile = pm_openr(argv[1]);
  fontimage = pnm_readpam(pamfile, &inpam, PAM_STRUCT_SIZE(tuple_type));
  pm_close(pamfile);
  printf("done\n");

  // Convert from PAM to viewport
  for (int x = 0; x < inpam.width; x++) {
    for (int y = 0; y < inpam.height; y++) {
      printf("(%d, %d) = R%d G%d B%d\n", x, y, fontimage[y][x][0], fontimage[y][x][1], fontimage[y][x][2]);
    }
  }

  // Send to Arduino
  //sendimage(image);

  pnm_freepamarray( fontimage, &inpam );

  return 0;
}

