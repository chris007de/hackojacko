// chackojacko.h
#ifndef HACKOJACKO_H
#define HACKOJACKO_H

#include "FastLED.h"

class Hackojacko {
private:
	unsigned int m_startLED;
	unsigned int m_stopLED;
	CRGB* m_LEDs;

public:
	Hackojacko(	CRGB*,
							unsigned int,		/* Start LED */
							unsigned int);	/* Stop LED */
	// Effects
	void setColor(CRGB);
  void runningLight(CRGB,
  									unsigned int /* period */);

};


#endif //HACKOJACKO_H