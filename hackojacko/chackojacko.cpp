// chackojacko.cpp

#include "chackojacko.h"

unsigned int checkLimit(unsigned int pLow,
                        unsigned int pTest,
                        unsigned int pHigh) {
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
                        unsigned int pStart,
                        unsigned int pStop):
  m_LEDs(pLEDs),
  m_startLED(pStart),
  m_stopLED(pStop)
  {}

void Hackojacko::setColor(CRGB pColor) {
  for(unsigned int i=m_startLED; i<=m_stopLED; i++) {
    m_LEDs[i] = (pColor);
  }
}

void Hackojacko::runningLight(CRGB pColor,
                              unsigned int pPeriod) {
  static unsigned long sTimer = millis();
  static unsigned int i = m_startLED;

  if((millis() - sTimer) > pPeriod) {
      sTimer = millis();
      if((i+1) > m_stopLED) {
        i = m_startLED;
      }
      else {
        i++;  
      }  
    }
  m_LEDs[checkLimit(m_startLED, (i-1), m_stopLED)] = CRGB::Black;
  m_LEDs[i] = pColor;	
  }
