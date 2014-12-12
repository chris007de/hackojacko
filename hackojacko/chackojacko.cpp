// chackojacko.cpp

#include "chackojacko.h"

unsigned int checkLimit(uint pLow,
                        uint pTest,
                        uint pHigh) {
  if(pTest < pLow) {
    return pLow;
  }
  else if(pTest > pHigh) {
    return pHigh;
  }
  else {
    return pTest;
  }
}

Hackojacko::Hackojacko(CRGB * pLEDs,
                        uint pStart,
                        uint pStop):
  m_LEDs(pLEDs),
  m_startLED(pStart),
  m_stopLED(pStop),
  m_state(P_ALL_OFF),
  m_period(100),
  m_color(CRGB::Black)
  {}

void Hackojacko::render(void) {
  switch(m_state) {
    case P_BLINK:
      blink();
      break;
    case P_RUN:
      running();
      break;
    case P_RANDOM:
      random();
    default:
      break;
  }
}

void Hackojacko::setColor(CRGB pColor) {
  m_color = pColor;
  if(P_ALL_ON == m_state) {
    setAll(m_color);
  }
}

void Hackojacko::off(void) {
  m_state = P_ALL_OFF;
  setAll(CRGB::Black);
}

void Hackojacko::on(void) {
  m_state = P_ALL_ON;
  setAll(m_color);  
}

void Hackojacko::setAll(CRGB pColor) {
  for(unsigned int i=m_startLED; i<=m_stopLED; i++) {
    m_LEDs[i] = (pColor);
    FastLED.show();
  }  
}

void Hackojacko::blink(void) {
  static unsigned long sTimer = millis();
  static bool sState = false;

  m_state = P_BLINK;

  if((millis() - sTimer) > m_period) {
      sTimer = millis();
      if(!sState) {
        setAll(m_color);

      }
      else {
        setAll(CRGB::Black);  
      }
      sState = !sState;
  }
}


void Hackojacko::running(void) {
  static unsigned long sTimer = millis();
  static uint i = m_startLED;

  m_state = P_RUN;

  if((millis() - sTimer) > m_period) {
      sTimer = millis();
      if((i+1) > m_stopLED) {
        i = m_startLED;
      }
      else {
        i++;  
      }  

  m_LEDs[checkLimit(m_startLED, (i-1), m_stopLED)] = CRGB::Black;
  m_LEDs[i] = m_color;  
  FastLED.show();
  }
}

#define NUM_PRESETS 4
static const CRGBPalette16 palettes[NUM_PRESETS] = {
    RainbowColors_p,
    RainbowStripeColors_p,
    CloudColors_p,
    PartyColors_p
  };

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void Hackojacko::random(bool pNextPreset) {
  static byte preset = 0;

  if(pNextPreset) {
    preset++;
  }
  if(preset >= NUM_PRESETS ) {
    preset = 0;
  }

  currentBlending = BLEND;
  currentPalette = palettes[preset];

  random();
}

void Hackojacko::random(void) {
  static unsigned long sTimer = millis();
  static uint8_t startIndex = 0;
  static uint8_t brightness = 255;
  uint8_t colorIndex;
  
  m_state = P_RANDOM;

  if((millis() - sTimer) > 10 /*m_period*/) {
    sTimer = millis();

    startIndex = startIndex + 1; /* motion speed */
    colorIndex = startIndex;
    for( int i = 0; i < NUM_LEDS; i++) {
      m_LEDs[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      colorIndex += 3;
    }
    FastLED.show();
  }
}
