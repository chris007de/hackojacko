#include "FastLED.h"
#include "SoftwareSerial.h"

#include "settings.h"

#include "chackojacko.h"


struct s_packet* readBT();
bool evalPacket(struct s_packet*);


// Define the array of leds
CRGB leds[NUM_LEDS];

int bluetoothTx = 2;  // TX-O pin of bluetooth module
int bluetoothRx = 3;  // RX-I pin of bluetooth module
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  Serial.begin(115200); // Default connection rate for my BT module
  bluetooth.begin(9600);  // The Bluetooth Mate defaults to 115200bps

  for(byte i=0; i<NUM_BUFFERS; i++) {
    packets[i].active = false;
  }
}

void loop() {
  evalPacket(readBT());

/*
  char msgType = -1;
  checkBT(&msgType);
  if(0x00 == msgType)
  {
     Serial.println("OFF!");
    leds[0] = CRGB::Black;
  }
  else if(0x03 == msgType)
  {
     Serial.println("ON!");
    leds[0] = CRGB::Red;
  }
  */
  /*
  if( (millis() - Timer) >= 500UL)
  {
    //leds[i++] = CRGB::Black;
    //leds[i] = CRGB(random(0,64),random(0,64),random(0,64));
    leds[i] = CRGB(r,g,b);
    if(NUM_LEDS == i)
    { i = 0; }
    Timer = millis();
  }
  */

  

}
/*
enum msg_read_states {
  S_READY_TO_READ,
  S_READ_HEADER1,
  S_READ_HEADER2,
  S_READ_HEADER3,
  S_READ_MSGTYPE,
  S_READ_MSGLEN_1,
  S_READ_MSGLEN_2,
  S_READ_CRC,
  S_READ_COLOR_R,
  S_READ_COLOR_G,
  S_READ_COLOR_B
};

void checkBT(char *pMsgType) {
  static enum msg_read_states fsm_state;
  char msgType;
  short msgLen;
  byte r = 0, g = 0, b = 0;
  bool setColors = false;
  fsm_state = S_READY_TO_READ;

  while(bluetooth.available() > 0){
    delay(10);
    char rxbyte = (char)bluetooth.read();

    switch (fsm_state) {
        case S_READY_TO_READ:
          if('A' == rxbyte) { fsm_state = S_READ_HEADER1; }
          else { fsm_state = S_READY_TO_READ;}
          break;
        case S_READ_HEADER1:
          if('J' == rxbyte) { fsm_state = S_READ_HEADER2; }
          else { fsm_state = S_READY_TO_READ;}
          break;
        case S_READ_HEADER2:
          if('A' == rxbyte) { fsm_state = S_READ_HEADER3; }
          else { fsm_state = S_READY_TO_READ;}
          break;
        case S_READ_HEADER3:
          if('B' == rxbyte) { fsm_state = S_READ_MSGTYPE; }
          else { fsm_state = S_READY_TO_READ;}
          break;
        case S_READ_MSGTYPE:
          msgType = rxbyte;
          fsm_state = S_READ_MSGLEN_1;
          break;
        case S_READ_MSGLEN_1:
          msgLen = rxbyte;
          fsm_state = S_READ_MSGLEN_2;
          break;
        case S_READ_MSGLEN_2:
          msgLen += rxbyte << 8;
          fsm_state = S_READ_CRC;
          break;
        case S_READ_CRC:
          // lol fuck this shit
          if(0x02 == msgType)
          {
            fsm_state = S_READ_COLOR_R;
          }
          else
          {
            fsm_state = S_READY_TO_READ;
          }
          break;
        case S_READ_COLOR_R:
          r = rxbyte;
          fsm_state = S_READ_COLOR_G;
          break;
        case S_READ_COLOR_G:
          g = rxbyte;
          fsm_state = S_READ_COLOR_B;
          break;
        case S_READ_COLOR_B:
          b = rxbyte;
          setColors = true;
          fsm_state = S_READY_TO_READ;
          break;
        default:
          // do something
          break;
    }

     //Serial.println(rxbyte+48);
  if(0x00 == msgType)
  {
    //Serial.println("OFF!");
    for(unsigned int i = 0; i< NUM_LEDS; i++)
    {
      leds[i] = CRGB::Black;
    } 
    FastLED.show();
  }
  else if(0x03 == msgType)
  {
     //Serial.println("ON!");
    leds[0] = CRGB::Red;
    FastLED.show();
  }
  else if( 0x02 == msgType && setColors)
  {
    Serial.println("");
    Serial.println(r,HEX);
    Serial.println(g,HEX);
    Serial.println(b,HEX);
    for(unsigned int i = 0; i< NUM_LEDS; i++)
    {
      leds[i] = CRGB(r,g,b);
    }    
    FastLED.show();
    setColors = false;
  }

  } 


}
*/


struct s_packet* readBT(void) {

  struct s_packet* packet;
  /*
  // look for free packet buffer
  for(byte i=0; i<NUM_BUFFERS; i++) {
    if(packets[i].active = false) {
      packet = &packets[i];
      break;
    }
    if(i == NUM_BUFFERS-1) {
      // no free buffer available
      return NULL;
    }
  } */
  packet = &packets[0];

  if(bluetooth.available() > 0) {
    delay(50); // DEBUG delay to read serial buffer
    enum rx_states fsm_state = S_READY_TO_READ;
    while(bluetooth.available() > 0)
    {
      switch (fsm_state) {
        case S_READY_TO_READ:
          if('A' == (char)bluetooth.read())
            { fsm_state = S_PREAMBLE1; }
          else { fsm_state = S_READY_TO_READ;}
          break;
        case S_PREAMBLE1:
          if('J' == (char)bluetooth.read())
            { fsm_state = S_PREAMBLE2; }
          else { fsm_state = S_READY_TO_READ;}
          break;
        case S_PREAMBLE2:
          if('A' == (char)bluetooth.read())
            { fsm_state = S_PREAMBLE3; }
          else { fsm_state = S_READY_TO_READ;}
          break;
        case S_PREAMBLE3:
          if('B' == (char)bluetooth.read())
            { fsm_state = S_READ_HEADER; }
          else { fsm_state = S_READY_TO_READ;}
          break;

        case S_READ_HEADER:
          // 1 byte MSG_TYPE
          packet->msgType = (char)bluetooth.read();
          // 2 byte MSG_LENGTH (MSB-LSB)
          packet->msgLen  = (char)bluetooth.read() << 8;
          packet->msgLen += (char)bluetooth.read();
          // 1 byte MSG_CRC
          packet->msgCRC  = (char)bluetooth.read();
          
          fsm_state = S_READ_BODY;
          break;

        case S_READ_BODY:      
          // clear rxBuffer
          memset(packet->rxBuffer, 0, sizeof(packet->rxBuffer));

          for(short i=0; 
              i<RXBUF_SIZE && i<(packet->msgLen); 
              i++)
          {
            packet->rxBuffer[i] = (char)bluetooth.read();
          }
          return packet;
          break;

        default:
          break;
        }

    }
  }
  return NULL;
}

/*
 * returns true if packet was 
 * evaluated successfully
 */
bool evalPacket(struct s_packet* pPacket) {
  if (NULL == pPacket) {
    return false;
  }

  byte r,g,b;

  switch (pPacket->msgType) {
      case 0x00: // PRESET
        switch (pPacket->rxBuffer[0]) {
            case 0x00: // ALL OFF
              memset(leds, 0, sizeof(leds));
              break;
            case 0x01: // ALL ON
              r = (byte)pPacket->rxBuffer[1];
              g = (byte)pPacket->rxBuffer[2];
              b = (byte)pPacket->rxBuffer[3];
              for(short i=0; i<NUM_LEDS; i++){
                  leds[i] = CRGB(r, g, b);
              }
            default:
              return false;
              break;
        }
        break;
      case 0x01: // 
        for(short i=0; i<NUM_LEDS;i++) {
          short idx = (i*3);
          if((idx+2) <= pPacket->msgLen)
          { // Read R,G,B from Message Body
            // regarding the length of the msg
            r = (byte)pPacket->rxBuffer[idx];
            g = (byte)pPacket->rxBuffer[idx + 1];
            b = (byte)pPacket->rxBuffer[idx + 2];
            leds[i] = CRGB(r, g, b);
          }
          else
          { // Turn of rest of LEDs
            leds[i] = CRGB::Black;
          }
        }
        break;
      default:
        return false;
        break;
  }
  pPacket->active = false;
  return true;
}
