#ifndef __TIMER_FACE__
#define __TIMER_FACE__

#include "StripArray.h"

class TimerFace : public StripArray {
private:
  uint8_t n_pixels;
  uint8_t last_row;
  int start_time;
  int end_time;
  bool running;
  uint8_t done_brightness;
  bool done_ascending;
public:
  TimerFace(int n, int l, int refresh_rate, Adafruit_NeoPixel *strip, CloudMsg *cm);
  void updateDisplay();
  void setTimer(int s, int e);
  void addPixels(uint8_t n);
  void resume(); //timer is set, but redraw board...
};
#endif
