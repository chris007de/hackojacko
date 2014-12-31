#include "FastLED.h"
#include "SoftwareSerial.h"

#include "settings.h"

#include "chackojacko.h"

// Define the array of leds
CRGB leds[NUM_LEDS];

Hackojacko myHackojacko(leds,0,NUM_LEDS-1);

struct s_packet rxBuffer[NUM_BUFFERS];
struct s_packet* readBT();
bool evalPacket(struct s_packet*);

//#define DEBUG

#ifdef DEBUG
#define DBG_PRINT(x, ...) Serial.print(x, ##__VA_ARGS__)
#define DBG_PRINTLN(x, ...) Serial.println(x, ##__VA_ARGS__)
#else
#define DBG_PRINT(x, ...)
#define DBG_PRINTLN(x, ...)
#endif


int bluetoothTx = 12;  // TX-O pin of bluetooth module
int bluetoothRx = 11;  // RX-I pin of bluetooth module
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(115200); // Default connection rate for my BT module
  bluetooth.begin(9600);  // The Bluetooth Mate defaults to 115200bps

  for(byte i=0; i<NUM_BUFFERS; i++) {
    rxBuffer[i].active = false;
  }
}

void loop() {

  if(evalPacket(readBT())) {
    FastLED.show();
  }
  myHackojacko.render();

}

/*
 * reads bluetooth buffer and evaluates 
 * packet
 * returns pointer to packet structure
 */
struct s_packet* readBT(void) {

  struct s_packet* packet;
  /*
  // look for free packet buffer
  for(byte i=0; i<NUM_BUFFERS; i++) {
    if(rxBuffer[i].active = false) {
      packet = &rxBuffer[i];
      break;
    }
    if(i == NUM_BUFFERS-1) {
      // no free buffer available
      return NULL;
    }
  } */
  packet = &rxBuffer[0];

  if(bluetooth.available() > 0) {
    delay(5); // DEBUG delay to read serial buffer
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
          // 2 byte MSG_LENGTH
          packet->msgLen  = (char)bluetooth.read();
          packet->msgLen += (char)bluetooth.read() << 8;
          // 1 byte MSG_CRC
          packet->msgCRC  = (char)bluetooth.read();

          // poor sanity checks
          if(0x00 != packet->msgCRC
            || 0x0F < packet->msgLen) {
            fsm_state = S_READY_TO_READ;
          }
          else
          {
            DBG_PRINT("MSG: 0x");
            DBG_PRINT(packet->msgType, HEX);
            DBG_PRINT(" LEN: 0x");
            DBG_PRINT(packet->msgLen, HEX);
            DBG_PRINT(" CRC: 0x");
            DBG_PRINTLN(packet->msgCRC, HEX);

            fsm_state = S_READ_BODY;
          }
          
          break;

        case S_READ_BODY:      
          // clear msgBody
          memset(packet->msgBody, 0, sizeof(packet->msgBody));

          for(short i=0; 
              i<RXBUF_SIZE && i<(packet->msgLen); 
              i++)
          {
            packet->msgBody[i] = (char)bluetooth.read();
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
      case T_PRESET:
        DBG_PRINTLN("  PRESET");
        switch (pPacket->msgBody[0]) {
            case P_ALL_OFF:
              DBG_PRINTLN("    ALL OFF");
              myHackojacko.off();
              break;
            case P_ALL_ON:
              DBG_PRINTLN("    ALL ON");
              myHackojacko.on();
              break;
            case P_BLINK:
              DBG_PRINTLN("    P_BLINK");
              myHackojacko.blink();
              break;
            case P_RUN:
              DBG_PRINTLN("    P_RUN");
              myHackojacko.running();
              break;
            case P_RANDOM:
              DBG_PRINTLN("    P_RANDOM");
              myHackojacko.random(true);
              break;
            default:
              DBG_PRINTLN("    unknown");
              return false;
              break;
        }
        break;
      case T_DIRECT: // DIRECT
        DBG_PRINTLN("  DIRECT");
        
        r = (byte)pPacket->msgBody[0];
        g = (byte)pPacket->msgBody[1];
        b = (byte)pPacket->msgBody[2];

        myHackojacko.setColor(CRGB(r, g, b));
        DBG_PRINT(" ");
        DBG_PRINT(r, DEC);
        DBG_PRINT(" ");
        DBG_PRINT(g, DEC);
        DBG_PRINT(" ");
        DBG_PRINTLN(b, DEC);
        break;

      case T_SPEED:
        DBG_PRINTLN("  SPEED");
        myHackojacko.setSpeed((byte)pPacket->msgBody[0]);

      default:
        return false;
        break;
  }
  pPacket->active = false;
  return true;
}
