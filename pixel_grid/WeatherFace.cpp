#include "WeatherFace.h"
#include "PixelStrip.h"
WeatherFace::WeatherFace(int n, int l, int refresh_rate, Adafruit_NeoPixel *strip, CloudMsg *cm) :
  StripArray(n, l, refresh_rate, strip, cm) {

  }

  void WeatherFace::track_temps(int * t, uint8_t *p) {
    temps = t;
    probs = p;
  }

  void WeatherFace::stop_tracking_temps() {
    temps = NULL;
  }

  void WeatherFace::updateDisplay() {
    PixelStrip *strips = getStrips();
    fill_array(0, 0, 0);
    for(uint8_t i = 0; i < numberOfStrips(); i++) {
      if(temps != NULL) {
        show_temp(i, temps[i]);
        get_anim_info(i);
        strips[i].render();
      }
    }
    render();
  }

  void WeatherFace::show_temp(uint8_t col, int temp) {
    //strips[col].gradient(0, 4, Pixel::color(0, 0, 100), Pixel::color(100, 0, 255));
    //strips[col].gradient(4, 8, Pixel::color(255, 100, 255), Pixel::color(0, 255, 0));
    //strips[col].gradient(8, 12, Pixel::color(100, 255, 0), Pixel::color(255, 0, 0));

    //strips[col].gradient(0, 4, Pixel::color(0, 0, 255), Pixel::color(0, 255, 150));
    //strips[col].gradient(4, 8, Pixel::color(0, 255, 0), Pixel::color(255, 255, 0));
    //strips[col].gradient(8, 12, Pixel::color(255, 200, 0), Pixel::color(255, 0, 0));

    uint8_t n;
    uint8_t t_max;
    uint8_t t_min = 0;
    temp = (temp < 110 ? temp : 110);
    temp = (temp > 0 ? temp : 0);
    PixelStrip *strips = getStrips();
    uint8_t half = stripLength() / 2;
    uint8_t all = stripLength();
    if(temp < 50) {
      t_max = 50;
      strips[col].gradient(0, half, Pixel::color(0, 0, 255), Pixel::color(0, 255, 150));
      strips[col].gradient(half, all, Pixel::color(0, 255, 100), Pixel::color(0, 255, 0));
    } else if(temp < 80) {
      t_min = 50;
      t_max = 80;
      strips[col].gradient(0, half, Pixel::color(0, 255, 0), Pixel::color(255, 255, 0));
      strips[col].gradient(half, all, Pixel::color(200, 255, 0), Pixel::color(255, 127, 0));
    } else {
      t_min = 80;
      t_max = 110;
      strips[col].gradient(0, half, Pixel::color(255, 127, 0), Pixel::color(255, 75, 0));
      strips[col].gradient(half, all, Pixel::color(255, 50, 0), Pixel::color(255, 0, 0));
    }

    double prop = (double(temp - t_min) / double(t_max - t_min));
    n = (uint8_t)(round(prop * double(stripLength())));
    n = (n > 0 ? n : 1);
    strips[col].clear(n, stripLength());
  }

  void WeatherFace::get_anim_info(uint8_t c) {
    uint8_t p = probs[c];
    PixelStrip *strips = getStrips();
    if(p < 40) {
      strips[c].set_n_spots(0);
    } else {
      double prop = 1 - (100.0 - p) / 60.0;
      uint8_t n = round(prop * 4) + 1;
      uint8_t s = 10 - round(prop * 10);
      strips[c].set_n_spots(n);
      strips[c].set_animation_delay(s);
    }
  }
