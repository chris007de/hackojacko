#include "FastLED.h"
#include "SoftwareSerial.h"

#include "settings.h"

#include "chackojacko.h"


void checkBT(char *pMsgType);

// Define the array of leds
CRGB leds[NUM_LEDS];

int bluetoothTx = 2;  // TX-O pin of bluetooth module
int bluetoothRx = 3;  // RX-I pin of bluetooth module
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

// 8 byte header
// + 50 * 3 byte max body length
#define RXBUF_SIZE 158
char rxBuffer[RXBUF_SIZE];


void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  Serial.begin(115200); // Default connection rate for my BT module
  bluetooth.begin(9600);  // The Bluetooth Mate defaults to 115200bps
}

void loop() {
  char msgType = -1;
  checkBT(&msgType);

/*
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

/* reads Message from BT Serial interface
 * returns amount of read bytes
 */
uint readBT(void) {
  if(bluetooth.available() > 0) {
    delay(50); // DEBUG delay to read serial buffer
    
    // clear rxBuffer
    memset(rxBuffer, 0, sizeof(rxBuffer));

    uint i = 0;
    while(bluetooth.available() > 0) {
      rxBuffer[i++];
    }
    return i;
  }
  return 0;
}

void evalRxBuf() {
  enum e_rxState{
    s_init,
    s_getHeader,
    s_getPreset,
    s_getDirect,
    s_finish,
    s_abort
  };

  char MSG_TYPE = -1;
  short MSG_LEN = -1;
  char CRC8     = -1;

  enum e_rxState rxState = s_init;
  while(s_abort != rxState
        && s_finish != rxState)
  {
    switch(rxState) {
      case s_init:
        // Check preamble
        if( 'A' == rxBuffer[0]
            && 'J' == rxBuffer[1]
            && 'A' == rxBuffer[2]
            && 'B' == rxBuffer[3])
        { rxState = s_getHeader; }
        else
        { rxState = s_abort; }
        break;

      case s_getHeader:
        // Read Header fields
        MSG_TYPE = rxBuffer[4];
        MSG_LEN  = rxBuffer[5] << 8;
        MSG_LEN += rxBuffer[6];
        CRC8     = rxBuffer[7];
        // validate Header content
        // TODO

        switch (MSG_TYPE) {
            case 0x00:
              rxState = s_getPreset;
              break;
            case 0x01:
              rxState = s_getDirect;
              break;
            default:
              break;
        }

        break;

      case s_getPreset:
        switch (rxBuffer[8]) {
            case 0x00:
              memset(leds, 0, sizeof(leds));
              break;
            default:
              break;
        }
        rxState = s_finish;
        break;

      case s_getDirect:
        for(short i=0; i<NUM_LEDS;i++) {
          short idx = HEADER_SIZE + (i*3);
          if((idx+2) <= MSG_LEN)
          { // Read R,G,B from Message Body
            // regarding the length of the msg
            byte r = (byte)rxBuffer[idx];
            byte g = (byte)rxBuffer[idx + 1];
            byte b = (byte)rxBuffer[idx + 2];
            leds[i] = CRGB(r, g, b);
          }
          else
          { // Turn of rest of LEDs
            leds[i] = CRGB::Black;
          }
        }
        rxState = s_finish;
        break;

      default:
        break;
    }
  }

}


