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
#define VIEWPORT_SIZE 96

typedef struct tagOPENED_FONT {
    int width;
    int height;
    uint8_t *image;
} OPENED_FONT;

typedef uint8_t VIEWPORT[3][8][4];

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

int getpixelindex(OPENED_FONT *font, int color, int x, int y) {
    return y + (x * font->width) + (color * font->width * 3);
}

void openfont(char *path, OPENED_FONT *font) {
    FILE *fontfile = NULL;
    struct pam fontpam;
    tuple *tuplerow;
    int row, column;

    fontfile = pm_openr(path);

    pnm_readpaminit(fontfile, &fontpam, PAM_STRUCT_SIZE(tuple_type));

    if ( ((fontpam.width % 8) != 0)
      || ((fontpam.height % 8) != 0)) {
        return;
    }

    uint8_t *image = malloc(fontpam.width * fontpam.height * 3 * sizeof(uint8_t) );

    font->image = image;
    font->width = fontpam.width;
    font->height = fontpam.height;

    if (image == NULL) {
        return;
    }

    tuplerow = pnm_allocpamrow(&fontpam);

    for (row = 0; row < 8; row++) {
        unsigned int column;
        pnm_readpamrow(&fontpam, tuplerow);
        for (column = 0; column < 8; column++) {
            // Green
            image[getpixelindex(font, 0, row, column)] = tuplerow[column][1];
            // Red
            image[getpixelindex(font, 1, row, column)] = tuplerow[column][0];
            // Blue
            image[getpixelindex(font, 2, row, column)] = tuplerow[column][2];
        }
    }

    pnm_freepamrow(tuplerow);

    pm_close(fontfile);
}

void closefont(OPENED_FONT *font) {
    free(font->image);
    font->width = 0;
    font->height = 0;
}

void getviewportfromfont(OPENED_FONT *font, VIEWPORT *viewport, int set, int character) {
    //TODO
}

void sendviewport(VIEWPORT *viewport) {
  struct sockaddr_in si_other;
  int s;

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  if (sendto(s, viewport, VIEWPORT_SIZE, 0,
     (struct sockaddr *) &si_other, sizeof(si_other))==-1)
    diep("sendto()");

  close(s);
}

int main(int argc, char *argv[])
{
    VIEWPORT viewport;
    OPENED_FONT font;

    openfont(argv[1], &font);

    getviewportfromfont(&font, &viewport, 1, 22);
    sendviewport(&viewport);

    closefont(&font);
}

