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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define LISTEN_PORT 1026
#define BUFFERSIZE 10

EthernetDisplay::EthernetDisplay(char *address, int port)
{
    //Packet data
    char databuffer[BUFFERSIZE] = {'C', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
    int slen = 1;
  
    // Init variables
    displayCount = 0;
    
    // Create a socket
    if ((socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      throw "Could not create socket";
    
    // Machine we are sending to
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(LISTEN_PORT);
    if (inet_aton(address, &si_other.sin_addr)==0)
      throw "inet_aton() failed";
    
    // Address to listen on
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(LISTEN_PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // Bind the socket
    if (bind(socket,(struct sockaddr*) &si_me, sizeof(si_me))==-1)
      throw "Error binding socket";
    
    // Send the packet
    if (sendto(socket, databuffer, sizeof(char), 0, (struct sockaddr *) &si_other, slen)==-1)
     throw "Error sending packet";
    
    // Get response
    databuffer[0] = '\0';
    while (databuffer[0] != 'C') recvfrom(socket, databuffer, BUFFERSIZE, 0, NULL, 0);
    
    // Store the number of displays
    displayCount = databuffer[1];
}

EthernetDisplay::~EthernetDisplay()
{
    close(socket);
}

int EthernetDisplay::getDisplayCount() {
  return displayCount;
}

bool EthernetDisplay::operator==(const EthernetDisplay& other) const
{
    return true;
}

