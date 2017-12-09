/*
 * Array of pixel strips
 */

#ifndef __STRIP_ARRAY_H__
#define __STRIP_ARRAY_H__

#include "neopixel/neopixel.h"
#include "CloudMsg.h"
#include "Pixel.h"

class PixelStrip;

class StripArray {
  private:
    Adafruit_NeoPixel *np_strip;
    PixelStrip *strips;
    uint8_t n_strips;
    uint8_t l_strips;
    uint16_t n_pixels;
    uint16_t refresh_interval;
    unsigned long last_update;

  protected:
    PixelStrip *getStrips();
    CloudMsg *msg;

  public:
    StripArray(int n, int l, int refresh_rate, Adafruit_NeoPixel *strip, CloudMsg *cm);
    void fill_array(uint8_t r, uint8_t g, uint8_t b);
    void loop();
    void render();
    void begin();
    void show();
    Pixel *pixel_at(uint8_t x, uint8_t y);
    void fill_row(uint8_t row, uint8_t r, uint8_t g, uint8_t b);
    void fill_col(uint8_t col, uint8_t r, uint8_t g, uint8_t b);
    uint8_t stripLength();
    uint8_t numberOfStrips();
    virtual void updateDisplay() {};
    void setBrightness(uint8_t b);
    void noDataDisplay();
};

 #endif
