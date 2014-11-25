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
  pinMode(statusLED, OUTPUT);
  digitalWrite(statusLED, LOW);

  Serial.begin(115200); // Default connection rate for my BT module
  bluetooth.begin(9600);  // The Bluetooth Mate defaults to 115200bps

  Timer = millis();
}

void loop() {
  checkBT();
  // if the state is 0 the led will turn off
  if (state == '0') {
    digitalWrite(statusLED, LOW);
  }
  // if the state is 1 the led will turn on
  else if (state == '1') {
    digitalWrite(statusLED, HIGH);
  } 

  leds[0] = CRGB(r,g,b);

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

  FastLED.show();

}

void checkBT() {
  //if some data is sent, read it and save it in the state variable
  if(bluetooth.available() > 0){
    Serial.print((char)bluetooth.read());
  } 
}
