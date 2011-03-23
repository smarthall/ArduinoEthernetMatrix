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

#ifndef _VIEWPORT_H
#define _VIEWPORT_H
#include "viewport.h"
#endif

// Debugging switch
//#define DEBUG

/* For accessing pixels in the viewport */
#define INDEX(x,y,plane) (((x) / 2 ) + ((y) * 4) + ((plane) * 32))
#define HILOW(x,y,plane) ((x) % 2)
#define HINIBBLE(x) (x>>4)
#define LONIBBLE(x) (x & 0x0F)
#define COMBINE(h,l) (((h)<<4)+(l))

/* Is the value in the high or low nibble */
#define HIGH 0
#define LOW 1

/* RGB Locations for PC */
#define PC_RED 0
#define PC_GREEN 1
#define PC_BLUE 2

/* RGB Locations for Arduino */
#define AR_RED 1
#define AR_GREEN 0
#define AR_BLUE 2

/* PC -> Arduino Plane LUT */
const uint8_t planelut[] = {AR_RED, AR_GREEN, AR_BLUE};

void setval(viewport image, uint8_t x, uint8_t y, uint8_t plane, uint8_t val) {
  #ifdef DEBUG
    char HL;
  #endif
  uint8_t oldval, newval, newplane, setval;
  newplane = planelut[plane];
  oldval = image[INDEX(x,y,newplane)];
  setval = val / 16;

  if (HILOW(x,y,newplane)) {
    // LOW
    newval = COMBINE(HINIBBLE(oldval), setval);
    #ifdef DEBUG
      HL = 'L';
    #endif
  } else {
    // HIGH
    newval = COMBINE(setval, LONIBBLE(oldval));
    #ifdef DEBUG
      HL = 'H';
    #endif
  }

  #ifdef DEBUG
    printf("(%d, %d, %d) = [%2d%c] = %1x : %2x->%2x\n", x, y, plane, INDEX(x,y,newplane), HL, setval, image[INDEX(x,y,newplane)], newval);
  #endif
  image[INDEX(x,y,newplane)] = newval;
}

uint8_t getval(viewport image, uint8_t x, uint8_t y, uint8_t plane) {
  plane = planelut[plane];
  return image[INDEX(x,y,plane)];
}

// To implement
//void setpixel(viewport image, uint8_t x, uint8_t y, tuple val);
//tuple getpixel(viewport image, uint8_t x, uint8_t y);

viewport allocviewport() {
  viewport newvp = NULL;
  if ((newvp = malloc(VIEWPORT_SIZE)) == NULL) return NULL;
  memset(newvp, 0, VIEWPORT_SIZE);
  return newvp;
}

void freeviewport(viewport image) {
  free(image);
}

void sendimage(viewport image, char* srv_ip, int port) {
  struct sockaddr_in si_other;
  int s, slen=sizeof(si_other);

  //Write to socket
  #ifdef DEBUG
    printf("Opening Socket... ");
  #endif
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    return;

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(port);
  if (inet_aton(srv_ip, &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }
  #ifdef DEBUG
    printf("done\n");
  #endif

  #ifdef DEBUG
    printf("Sending packet... ");
  #endif
  if (sendto(s, image, VIEWPORT_SIZE, 0, (struct sockaddr *) &si_other, slen)==-1)
    return;
  #ifdef DEBUG
    printf("done\n");
  #endif

  #ifdef DEBUG
    printf("Closing socket... ");
  #endif
  close(s);
  #ifdef DEBUG
    printf("done\n");
  #endif
}

