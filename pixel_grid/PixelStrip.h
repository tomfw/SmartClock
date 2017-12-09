/*
 * A run of pixels in a row
 */

#ifndef __PIXEL_STRIP_H__
#define __PIXEL_STRIP_H__

#include "Particle.h"
#include "application.h"
#include "neopixel/neopixel.h"
#include "Pixel.h"
#include "CloudMsg.h"

class PixelStrip {
private:
  Adafruit_NeoPixel *np_strip;
  Pixel *pixels;
  uint8_t i_start;
  uint8_t n_pixels;  //number of pixels in run
  bool isReversed;      //reversed? for grid displays
  CloudMsg *msg;
  uint8_t n_spots;
  uint8_t *i_spots;
  uint8_t frames;
  uint8_t animation_delay;
public:
  PixelStrip() {}
  PixelStrip(Adafruit_NeoPixel *s, uint8_t i, uint8_t len, bool reversed, CloudMsg *m);
  ~PixelStrip();
  uint8_t *get_pixel(uint8_t index); //get rgb (grid ordered)
  uint8_t *get_ordered_pixel(uint8_t index); //get rgb (strip ordered)
  void set_pixel(uint8_t index, uint8_t *pixel);
  void fill_solid(uint8_t r, uint8_t g, uint8_t b);
  int convert_index(uint8_t index);
  Pixel *pixel_at(uint8_t index);
  void render();
  void gradient(uint8_t start, uint8_t end, uint8_t *c_start, uint8_t *c_end);
  void clear(uint8_t start, uint8_t end);
  void scale_color(uint8_t *color, uint8_t brightness);
  void set_animation_delay(uint8_t d);
  void set_n_spots(uint8_t n);
};

#endif
