#include "Pixel.h"

Pixel::Pixel(Adafruit_NeoPixel *s, uint8_t i, CloudMsg *m) {
  np_strip = s;
  index = i;
  msg = m;
  brightness = 50;
}

void Pixel::set_brightness(uint8_t b) {
  brightness = b;
}

uint8_t Pixel::get_brightness() {
  return brightness;
}

void Pixel::set(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t *color = Pixel::color(r, g, b);
    scale_color(color, brightness);
    np_strip->setPixelColor(index, color[0], color[1], color[2]);
    free(color);
}

uint32_t Pixel::get_color() {
  return np_strip->getPixelColor(index);
}

bool Pixel::isOn() {
  return get_color() > 1;
}
void Pixel::scale_color(uint8_t *color, uint8_t brightness) {
  for(uint8_t i = 0; i < 3; i++) {
    color[i] = (double(color[i]) / 255.0) * brightness;
  }
}


uint8_t* Pixel::color(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t *color = new uint8_t[3];
  color[0] = r;
  color[1] = g;
  color[2] = b;
  return color;
}
