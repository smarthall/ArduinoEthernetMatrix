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


#ifndef ETHERNETDISPLAY_H
#define ETHERNETDISPLAY_H

#include <stdint.h>
#include <netinet/in.h>

#include <cstring>
#include <string>

/* RGB Locations for PC */
#define PC_RED 0
#define PC_GREEN 1
#define PC_BLUE 2

class EthernetDisplay {
    uint8_t **viewports;
    int socket_h, displayCount;
    struct sockaddr_in si_control, si_img, si_me;
    int x, y;
    
  public:
    EthernetDisplay(std::string address);
    virtual ~EthernetDisplay();
    virtual bool operator==(const EthernetDisplay& other) const;
    
    //Accessors
    int getDisplayCount();
    int getXSize();
    int getYSize();
    
    //Pixel functions
    void setval(uint8_t x, uint8_t y, uint8_t plane, uint8_t val);
    uint8_t getval(uint8_t x, uint8_t y, uint8_t plane);
    
    //Network Functions
    void sync();
    void sync(int display);
};

#endif // ETHERNETDISPLAY_H
