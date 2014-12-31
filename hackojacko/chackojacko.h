// chackojacko.h
#ifndef HACKOJACKO_H
#define HACKOJACKO_H

#include "FastLED.h"
#include "settings.h"

class Hackojacko {
private:
	CRGB* m_LEDs;
	uint m_startLED;
	uint m_stopLED;
	CRGB m_color;
	uint m_period;

	short m_state;

public:
	Hackojacko(	CRGB*,
							uint,		/* Start LED */
							uint);	/* Stop LED */
	// Effects
	void render(void);

	void setColor(CRGB);
	void setAll(CRGB);

	void setSpeed(byte);

	void off(void);
	void on(void);
	void blink(void);
  void running(void);
  void random(bool);
  void random(void);

};


#endif //HACKOJACKO_H