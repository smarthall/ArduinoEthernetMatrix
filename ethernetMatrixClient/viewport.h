#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#ifdef __APPLE__
  #include <pam.h>
#else
  #include <netpbm/pam.h>
#endif

#define VIEWPORT_SIZE (sizeof(uint8_t) * 97)
typedef uint8_t* viewport;

/* Viewport management functions */
viewport allocviewport(uint8_t display);
void freeviewport(viewport image);

/* Viewport editing */
void sendimage(viewport image, char* srv_ip, int port);
void setval(viewport image, uint8_t x, uint8_t y, uint8_t plane, uint8_t val);
uint8_t getval(viewport image, uint8_t x, uint8_t y, uint8_t plane);
void setpixel(viewport image, uint8_t x, uint8_t y, tuple val);
tuple getpixel(viewport image, uint8_t x, uint8_t y);

