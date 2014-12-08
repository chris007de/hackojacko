#include "FastLED.h"
#include "SoftwareSerial.h"

#include "settings.h"

#include "chackojacko.h"

// Define the array of leds
CRGB leds[NUM_LEDS];

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
    packets[i].active = false;
  }
}

void loop() {

  if(evalPacket(readBT())) {
    FastLED.show();
  }

}



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
      case T_PRESET:
        DBG_PRINTLN("  PRESET");
        switch (pPacket->rxBuffer[0]) {
            case P_ALL_OFF:
              DBG_PRINTLN("    ALL OFF");
              memset(leds, 0x00, sizeof(leds));
              break;
            case P_ALL_ON:
              DBG_PRINTLN("    ALL ON");
              memset(leds, 0xFF, sizeof(leds));
            case P_BLINK:
              DBG_PRINTLN("    P_BLINK");
              break;
            case P_RUN:
              DBG_PRINTLN("    P_RUN");
              break;
            case P_RANDOM:
              DBG_PRINTLN("    P_RANDOM");
              break;
            default:
              DBG_PRINTLN("    unknown");
              return false;
              break;
        }
        break;
      case T_DIRECT: // DIRECT
        DBG_PRINTLN("  DIRECT");

        r = (byte)pPacket->rxBuffer[0];
        g = (byte)pPacket->rxBuffer[1];
        b = (byte)pPacket->rxBuffer[2];
        for(short i=0; i<NUM_LEDS; i++){
            leds[i] = CRGB(r, g, b);
        }
        DBG_PRINT(" ");
        DBG_PRINT(r, DEC);
        DBG_PRINT(" ");
        DBG_PRINT(g, DEC);
        DBG_PRINT(" ");
        DBG_PRINTLN(b, DEC);
        break;

      default:
        return false;
        break;
  }
  pPacket->active = false;
  return true;
}
