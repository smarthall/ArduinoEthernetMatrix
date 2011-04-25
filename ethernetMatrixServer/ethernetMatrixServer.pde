#include <Wire.h>
#include <SoftwareSerial.h>
#include "WProgram.h"
#include "etherShield.h"

#define BUFFER_SIZE 250
#define SERIAL_BUFFER 128
#define SENDBUF_LEN 5
#define SCAN_INTERVAL 500

#define INDICATOR_PIN    7

#define STATE_WAITING    0
#define STATE_STARTCODE  1
#define STATE_COMMAND    2
#define STATE_ADDRESS    3
#define STATE_DATALEN    4
#define STATE_DATA       5

// Definitions
#define START_CODE 0x1C

#define FRAME_CMD 0x46
#define SCAN_CMD  0x45

uint8_t sbuf[SERIAL_BUFFER];
uint8_t serial_state = STATE_WAITING;
uint8_t sdata_p, scmd, saddress, sdatalen, validpacket = 0;

// Settings
static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static uint8_t myip[4]  = {192,168,1,15};

// State
uint8_t displaycount = 0;
uint8_t lockip[]     = {0,0,0,0};
uint8_t locked       = 0;

unsigned long ledoff = 0; // Time to turn off the indicator
unsigned long nextScan = 0;

uint8_t buf[BUFFER_SIZE+1];
uint8_t sendbuf[SENDBUF_LEN];
uint16_t plen, data_p;

EtherShield es = EtherShield();

void setup() {
  // Set the indicator light
  pinMode(INDICATOR_PIN, OUTPUT);
  digitalWrite(INDICATOR_PIN, HIGH);
  
  // Setup Serial
  Serial.begin(115200);
  
  // Setup Ethernet
  setupEthernet();
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
       && buf[UDP_LEN_H_P] == 0 && buf[UDP_LEN_L_P] == 105 ) {
         // Send to children
         sendPacket(FRAME_CMD, buf[UDP_DATA_P], 0x60, buf + (UDP_DATA_P + 1));
      }
      
      if (buf[IP_PROTO_P] == IP_PROTO_UDP_V && buf[UDP_DST_PORT_H_P] == 4 && buf[UDP_DST_PORT_L_P] == 2
       && buf[UDP_LEN_H_P] == 0) {
         if (buf[UDP_DATA_P] == 'C') {
           // Send display count back
           sendbuf[0] = 'C';
           sendbuf[1] = displaycount;
           es.ES_make_udp_reply_from_request(buf, sendbuf, sizeof(uint8_t) * 2, 1026);
         }
      }
    }
  }
  
  if (Serial.available() > 0) {
    byte data = Serial.read();
    
    checkForPacket(data);
    
    if (validpacket) {
      switch (scmd) {
        case SCAN_CMD:
          if (sdatalen == 1) displaycount = sbuf[0];
          digitalWrite(INDICATOR_PIN, LOW);
        break;
      }
     
      // Packet is dealt with
      validpacket = 0;
    }
  }
  
  if ((displaycount == 0) && (nextScan < millis())) {
    byte startZero = 0x00;
    sendPacket(SCAN_CMD, 0xFF, 0x01, &startZero);
    nextScan = millis() + SCAN_INTERVAL;
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

void sendPacket(byte command, byte address, byte datalen, byte *data) {
  byte checksum;
  
  // Write out the command
  Serial.write((byte)START_CODE);
  Serial.write((byte)command);
  Serial.write((byte)address);
  Serial.write((byte)datalen);
  Serial.write(data, datalen);
  
  // Calculate checksum
  checksum = START_CODE;
  checksum ^= command;
  checksum ^= address;
  checksum ^= datalen;
  for (int i = 0; i < datalen; i++) {
    checksum ^= data[i];
  }
  
  // Write checksum
  Serial.write((byte)checksum);
}

void checkForPacket(byte data) {
  static byte checksum;
  checksum ^= data;
  
  switch (serial_state) {
    case STATE_WAITING:
       checksum = START_CODE;
       if (data == START_CODE) serial_state = STATE_STARTCODE;
    break;
    case STATE_STARTCODE:
      scmd = data;
      serial_state = STATE_COMMAND;
    break;
    case STATE_COMMAND:
      saddress = data;
      serial_state = STATE_ADDRESS;
    break;
    case STATE_ADDRESS:
      sdatalen = data;
      sdata_p = 0;
      serial_state = STATE_DATALEN;
    break;
    case STATE_DATALEN:
      *(sbuf + sdata_p++) = data;
      if (sdata_p == sdatalen)
        serial_state = STATE_DATA;
    break;
    case STATE_DATA:
      // Checksum has been XORed with itself by now, should be zero
      if (checksum == 0x00) validpacket = 1;
      serial_state = STATE_WAITING;
    break;
  }
}

