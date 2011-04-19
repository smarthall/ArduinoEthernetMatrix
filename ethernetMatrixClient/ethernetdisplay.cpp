/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "ethernetdisplay.h"

//Dependencies
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define CONTROL_PORT 1026
#define IMG_PORT 1025
#define BUFFERSIZE 10
#define DISPLAYSIZE 96

/* For accessing pixels in the viewport */
#define INDEX(x,y,plane) (((x) / 2 ) + ((y) * 4) + ((plane) * 32))
#define HILOW(x,y,plane) ((x) % 2)
#define HINIBBLE(x) (x>>4)
#define LONIBBLE(x) (x & 0x0F)
#define COMBINE(h,l) (((h)<<4)+(l))

/* Is the value in the high or low nibble */
#define HIGH 0
#define LOW 1

/* RGB Locations for Arduino */
#define AR_RED 1
#define AR_GREEN 0
#define AR_BLUE 2

/* PC -> Arduino Plane LUT */
const uint8_t planelut[] = {AR_RED, AR_GREEN, AR_BLUE};

EthernetDisplay::EthernetDisplay(std::string address)
{
    // TODO: Split the constructor into a few parts
    //Packet data
    char databuffer[BUFFERSIZE] = {'C', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
  
    // Init variables
    displayCount = 0;
    viewports = NULL;
    
    // Create a socket
    if ((socket_h=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      throw "Could not create socket";
    
    // Sending control data to
    memset((char *) &si_control, 0, sizeof(si_control));
    si_control.sin_family = AF_INET;
    si_control.sin_port = htons(CONTROL_PORT);
    if (inet_aton(address.c_str(), &si_control.sin_addr)==0)
      throw "inet_aton() failed";
    
    // Sending images to
    memset((char *) &si_img, 0, sizeof(si_img));
    si_img.sin_family = AF_INET;
    si_img.sin_port = htons(IMG_PORT);
    if (inet_aton(address.c_str(), &si_img.sin_addr)==0)
      throw "inet_aton() failed";
    
    // Address to listen on
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(CONTROL_PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // Bind the socket
    if (bind(socket_h,(struct sockaddr*) &si_me, sizeof(si_me))==-1)
      throw "Error binding socket";
    
    // TODO: Abstract the sending of commands
    // Send the packet
    if (sendto(socket_h, databuffer, sizeof(char), 0, (struct sockaddr *) &si_control
, sizeof(si_control
))==-1)
     throw "Error sending packet";
    
    // Get response
    // TODO: Timeout after a few seconds
    // TODO: Abstract out the recieving of commands
    databuffer[0] = '\0';
    while (databuffer[0] != 'C') recvfrom(socket_h, databuffer, BUFFERSIZE, 0, NULL, 0);
    
    // Store the number of displays
    displayCount = databuffer[1];

    // Setup viewports
    viewports = (uint8_t **) malloc(sizeof(uint8_t*) * displayCount);
    for (int i = 0; i < displayCount; i++) {
        viewports[i] = (uint8_t*) malloc(sizeof(uint8_t) * DISPLAYSIZE);
	viewports[i][0] = displayCount - i - 1;
    }

    // Calculate coordinates
    y = 8;
    x = 8 * displayCount;
}

EthernetDisplay::~EthernetDisplay()
{
    // Free the viewports
    for (int i = 0; i < displayCount; i++) {
        free(viewports[i]);
    }
    free(viewports);

    // Close the socket
    close(socket_h);
}

int EthernetDisplay::getDisplayCount() {
  return displayCount;
}

bool EthernetDisplay::operator==(const EthernetDisplay& other) const
{
    // TODO: Compare the address and port
    return true;
}

int EthernetDisplay::getXSize() {
    return x;
}

int EthernetDisplay::getYSize() {
    return y;
}

void EthernetDisplay::setval(uint8_t x, uint8_t y, uint8_t plane, uint8_t val) {
    int vp = x / 8;
    x = x % 8;
    
    uint8_t oldval, newval, newplane, setval;
    newplane = planelut[plane];
    oldval = viewports[vp][INDEX(x,y,newplane)];
    setval = val / 16;

    if (HILOW(x,y,newplane)) {
      // LOW
      newval = COMBINE(HINIBBLE(oldval), setval);
    } else {
      // HIGH
      newval = COMBINE(setval, LONIBBLE(oldval));
    }

    viewports[vp][INDEX(x,y,newplane)] = newval;
}

uint8_t EthernetDisplay::getval(uint8_t x, uint8_t y, uint8_t plane) {
    int vp = x / 8;
    x = x % 8;
    
    plane = planelut[plane];
    return viewports[vp][INDEX(x,y,plane)];
}

void EthernetDisplay::fromraw(uint8_t display, uint8_t *viewport) {
    memcpy(viewports[display], viewport, DISPLAYSIZE * sizeof(uint8_t));
}

void EthernetDisplay::sync() {
  for (int i = 0; i < displayCount; i++) {
    sync(i);
  }
}

void EthernetDisplay::sync(int display) {
    uint8_t buf[DISPLAYSIZE + 1];
    
    buf[0] = displayCount - display - 1;
    memcpy(buf + 1, viewports[display], sizeof(uint8_t) * DISPLAYSIZE);
    
    if (sendto(socket_h, buf, DISPLAYSIZE + 1, 0, (struct sockaddr *) &si_img, sizeof(si_img))!=0)
      return;
}

    

