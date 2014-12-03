#include "FastLED.h"
#include "SoftwareSerial.h"

#include "settings.h"

#include "chackojacko.h"


void checkBT(char *pMsgType);

// 8 byte header
// + 50 * 3 byte max body length
#define RXBUF_SIZE 158
struct s_packet{
  char msgType;
  short msgLen;
  char msgCRC;
  char rxBuffer[RXBUF_SIZE];
  };

struct s_packet packets[3];

// Define the array of leds
CRGB leds[NUM_LEDS];

int bluetoothTx = 2;  // TX-O pin of bluetooth module
int bluetoothRx = 3;  // RX-I pin of bluetooth module
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

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
enum fsm_states{
  S_READY_TO_READ,
  S_PREAMBLE1,
  S_PREAMBLE2,
  S_PREAMBLE3,
  S_READ_HEADER,
  S_READ_BODY
};
/* reads Message from BT Serial interface
 * returns amount of read bytes
 */
bool readBT(void) {

  if(bluetooth.available() > 0) {
    delay(50); // DEBUG delay to read serial buffer
    enum fsm_states fsm_state = S_READY_TO_READ;
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
          packets[0].msgType = (char)bluetooth.read();
          // 2 byte MSG_LENGTH (MSB-LSB)
          packets[0].msgLen  = (char)bluetooth.read() << 8;
          packets[0].msgLen += (char)bluetooth.read();
          // 1 byte MSG_CRC
          packets[0].msgCRC  = (char)bluetooth.read();
          
          fsm_state = S_READ_BODY;
          break;

        case S_READ_BODY:      
          // clear rxBuffer
          memset(packets[0].rxBuffer, 0, sizeof(packets[0].rxBuffer));

          for(short i=0; 
              i<RXBUF_SIZE && i<(packets[0].msgLen); 
              i++)
          {
            packets[0].rxBuffer[i] = (char)bluetooth.read();
          }
          return true;
          break;

        default:
          break;
        }

    }
  }
  return false;
}

void evalPacket() {

  switch (packets[0].msgType) {
      case 0x00:
        switch (packets[0].rxBuffer[0]) {
            case 0x00:
              memset(leds, 0, sizeof(leds));
              break;
            default:
              break;
        }
        break;
      case 0x01:
        for(short i=0; i<NUM_LEDS;i++) {
          short idx = (i*3);
          if((idx+2) <= packets[0].msgLen)
          { // Read R,G,B from Message Body
            // regarding the length of the msg
            byte r = (byte)packets[0].rxBuffer[idx];
            byte g = (byte)packets[0].rxBuffer[idx + 1];
            byte b = (byte)packets[0].rxBuffer[idx + 2];
            leds[i] = CRGB(r, g, b);
          }
          else
          { // Turn of rest of LEDs
            leds[i] = CRGB::Black;
          }
        }
        break;
      default:
        break;
  }
}
