
#ifndef __PIXEL_H__
#define __PIXEL_H__

#include "Particle.h"
#include "application.h"
#include "neopixel/neopixel.h"
#include "CloudMsg.h"

class Pixel {
  private:
    Adafruit_NeoPixel *np_strip;
    CloudMsg *msg;
    uint8_t index;
    uint8_t brightness;
  public:
    Pixel() {}
    Pixel(Adafruit_NeoPixel *s, uint8_t i, CloudMsg *m);
    void set(uint8_t r, uint8_t g, uint8_t b);
    uint32_t get_color();
    static uint8_t *color(uint8_t r, uint8_t g, uint8_t b);
    void scale_color(uint8_t *color, uint8_t brightness);
    uint8_t get_brightness();
    void set_brightness(uint8_t b);
    bool isOn();
};
#endif
