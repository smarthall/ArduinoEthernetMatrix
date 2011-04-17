#include <stdlib.h>
#include <stdio.h>

#include "ethernetdisplay.h"

#define PORT 1026
#define SRV_IP "192.168.1.15"

using namespace std;

int main(int argc, char *argv[])
{
  try {
    EthernetDisplay e = EthernetDisplay(SRV_IP, PORT);
    
    // Tell the user
    printf("Displays: %d\n", e.getDisplayCount());
    
  } catch (const char *errmsg) {
    std::cerr << "Ethernet Matrix Error: " << errmsg << "\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

