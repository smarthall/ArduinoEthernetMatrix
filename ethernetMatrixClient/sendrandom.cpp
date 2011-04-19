#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "ethernetdisplay.h"

#define BUFLEN 96
#define SRV_IP "192.168.1.15"

int main(int argc, char *argv[])
{
  FILE *randomfile = NULL;
  uint8_t buf[BUFLEN];
  EthernetDisplay e = EthernetDisplay(SRV_IP);
  
  // Open Random file
  randomfile = fopen("/dev/urandom", "r");
  if (randomfile == NULL) {
      return EXIT_FAILURE;
  }
  
  // Set displays to random data
  for (int i=0; i < e.getDisplayCount(); i++) {
    fread(buf, sizeof(char), 96, randomfile);
    e.fromraw(i, buf);
  }

  // Update displays
  e.sync();

  return EXIT_SUCCESS;
}

