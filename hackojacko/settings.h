#ifndef SETTINGS_H
#define SETTINGS_H

#define NUM_LEDS 50

#define DATA_PIN 12

typedef unsigned int uint;

#define NUM_BUFFERS 3
// 50 * 3 byte max body length
#define RXBUF_SIZE 150

struct s_packet{
  bool active;
  char msgType;
  short msgLen;
  char msgCRC;
  char rxBuffer[RXBUF_SIZE];
  }packets[NUM_BUFFERS];

enum rx_states{
  S_READY_TO_READ,
  S_PREAMBLE1,
  S_PREAMBLE2,
  S_PREAMBLE3,
  S_READ_HEADER,
  S_READ_BODY
};



#endif // SETTINGS_H