#ifndef SETTINGS_H
#define SETTINGS_H

#define NUM_LEDS 20
#define BRIGHTNESS 64

#define DATA_PIN 10

typedef unsigned int uint;

// MESSAGE TYPES
#define T_PRESET  0x00
  #define P_ALL_OFF 0x00
  #define P_ALL_ON  0x01
  #define P_BLINK   0x02
  #define P_RUN     0x03
  #define P_RANDOM  0x04

#define T_DIRECT  0x01
// MESSAGE TYPES

#define NUM_BUFFERS 1
// 50 * 3 byte max body length
#define RXBUF_SIZE 150

struct s_packet{
  bool active;
  char msgType;
  short msgLen;
  char msgCRC;
  char msgBody[RXBUF_SIZE];
  };

enum rx_states{
  S_READY_TO_READ,
  S_PREAMBLE1,
  S_PREAMBLE2,
  S_PREAMBLE3,
  S_READ_HEADER,
  S_READ_BODY
};



#endif // SETTINGS_H