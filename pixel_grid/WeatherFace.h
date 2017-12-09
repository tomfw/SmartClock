#ifndef __WEATHER_FACE__
#define __WEATHER_FACE__

#include "StripArray.h"
#include "CloudMsg.h"
class WeatherFace : public StripArray {
private:
  bool trackingTemps;
  int *temps;
  uint8_t *probs;
public:
  WeatherFace(int n, int l, int refresh_rate, Adafruit_NeoPixel *strip, CloudMsg *cm);
  void show_temp(uint8_t col, int temp);
  void track_temps(int *t, uint8_t *p);
  void stop_tracking_temps();
  void get_anim_info(uint8_t c);
  void updateDisplay();
};

#endif
