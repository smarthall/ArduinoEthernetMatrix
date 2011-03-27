#include <Wire.h>
#include <SoftwareSerial.h>
#include "WProgram.h"
#include "etherShield.h"

// Definitions
#define FRAME_START 0x46
#define INIT_START  0x45
#define BUFFER_SIZE 250

#define STATE_WAITING    0
#define STATE_DISPCOUNT  1

// Settings
static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static uint8_t myip[4]  = {192,168,1,15};

// State
uint8_t displaycount = 0;
uint8_t serial_state = STATE_WAITING;
uint8_t lockip       = {0,0,0,0};
uint8_t locked       = 0;

unsigned char buf[BUFFER_SIZE+1];
uint16_t plen, data_p;

EtherShield es = EtherShield();

void setup() {  
  // Setup Serial
  Serial.begin(115200);
  
  // Setup Ethernet
  setupEthernet();
  
  // Wait for child rainbowduinos to come online
  delay(4000);
  
  // Send the init command to the children
  Serial.write((byte)INIT_START);
  Serial.write((byte)0x00); // Start displays at 0
}

void loop() {
  plen = es.ES_enc28j60PacketReceive (BUFFER_SIZE, buf);

  /*plen will be unequal to zero if there is a valid packet (without crc error) */
  if (plen != 0){
    if (es.ES_eth_type_is_arp_and_my_ip (buf,plen)) {
      es.ES_make_arp_answer_from_request (buf);
    }
    // check if ip packets (icmp or udp) are for us:
    if (es.ES_eth_type_is_ip_and_my_ip (buf,plen)!=0) {
      if (buf[IP_PROTO_P] == IP_PROTO_ICMP_V && buf[ICMP_TYPE_P] == ICMP_TYPE_ECHOREQUEST_V) {
        // a ping packet, let's send pong
        es.ES_make_echo_reply_from_request (buf, plen);
      }
      
      if (buf[IP_PROTO_P] == IP_PROTO_UDP_V && buf[UDP_DST_PORT_H_P] == 4 && buf[UDP_DST_PORT_L_P] == 1
       && buf[UDP_LEN_L_P] == 105 && buf[UDP_LEN_H_P] == 0) {
        Serial.write((byte)FRAME_START); // Start Code (for a frame)
        Serial.write(buf[UDP_DATA_P]); // Address
        Serial.write((byte)0x60); // Data Length
        Serial.write(buf + (UDP_DATA_P + 1), 96); // Data
        Serial.write((byte)0x00); // Checksum - Fake it till you make it :D
      }
      
      if (buf[IP_PROTO_P] == IP_PROTO_UDP_V && buf[UDP_DST_PORT_H_P] == 4 && buf[UDP_DST_PORT_L_P] == 2
       && buf[UDP_LEN_L_P] == 105 && buf[UDP_LEN_H_P] == 0) {
         // Commands recieved here
      }
    }
  }
  
  if (Serial.available() != 0) {
    byte data = Serial.read();
    
    select (serial_state) {
      case STATE_WAITING:
        if (data == INIT_START) serial_state = STATE_DISPCOUNT;
      break;
      case STATE_DISPCOUNT:
        displaycount = data;
        serial_state = STATE_WAITING;
      break;
    }
  }
}

/* ================================================ */
/* Ethernet Functions */
void setupEthernet() {
  /*initialize enc28j60*/
  es.ES_enc28j60Init (mymac);
  es.ES_enc28j60clkout (2); // change clkout from 6.25MHz to 12.5MHz
  es.ES_enc28j60PhyWrite (PHLCON, 0x476);
  delay (10);
  es.ES_init_ip_arp_udp_tcp (mymac, myip, 80);
}

