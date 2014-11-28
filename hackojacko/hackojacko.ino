#include "FastLED.h"
#include "SoftwareSerial.h"

#include "settings.h"

#include "chackojacko.h"

int state = '1';

byte r=0;
byte g=0;
byte b=0;

int i = 0;
unsigned long Timer;

// Define the array of leds
CRGB leds[NUM_LEDS];

int bluetoothTx = 2;  // TX-O pin of bluetooth module
int bluetoothRx = 3;  // RX-I pin of bluetooth module

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // sets the pins as outputs:
  pinMode(DEBUG_PIN, OUTPUT);
  digitalWrite(DEBUG_PIN, LOW);

  Serial.begin(115200); // Default connection rate for my BT module
  bluetooth.begin(9600);  // The Bluetooth Mate defaults to 115200bps

  Timer = millis();
}
void checkBT(char *pMsgType);

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


