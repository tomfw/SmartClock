#ifndef __CLOCK_FACE__
#define __CLOCK_FACE__

#include "StripArray.h"

class ClockFace : public StripArray {
private:
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  bool isPM;
  bool havePrinted;
public:
  ClockFace(int n, int l, int refresh_rate, Adafruit_NeoPixel *strip, CloudMsg *cm);
  void updateDisplay();
  void drawFace(uint8_t r, uint8_t g, uint8_t b);
  void drawTime();
  void updateTime();
  uint8_t x_minute(uint8_t min);
  uint8_t x_hour(uint8_t hour);
  uint8_t y_minute(uint8_t min);
  uint8_t y_hour(uint8_t hour);
  void drawHand(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
  void traceTo(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
};
#endif
