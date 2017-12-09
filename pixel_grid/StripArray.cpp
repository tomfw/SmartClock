#include "StripArray.h"
#include "PixelStrip.h"

StripArray::StripArray(int n, int l, int refresh_rate, Adafruit_NeoPixel *strip, CloudMsg *cm) {
  n_strips = n;
  l_strips = l;
  n_pixels = n * l;
  last_update = 0;
  refresh_interval = 1000 / refresh_rate;
  msg = cm;
  np_strip = strip;
  strips = new PixelStrip[n_strips];
  bool rev = false;
  for(int i = 0; i < n_strips; i++) {
    strips[i] = PixelStrip(np_strip, i * l_strips, l_strips, rev, msg);
    if(rev) rev = false;
    else rev = true;
  }
}

void StripArray::fill_col(uint8_t col, uint8_t r, uint8_t g, uint8_t b) {
  //fill one "PixelStrip"
  strips[col].fill_solid(r, g, b);
}

void StripArray::fill_row(uint8_t row, uint8_t r, uint8_t g, uint8_t b) {
  Pixel *current;
  for(uint8_t i = 0; i < n_strips; i++) {
    current = pixel_at(i, row);
    current->set(r, g, b);
  }
}

void StripArray::fill_array(uint8_t r, uint8_t g, uint8_t b) {
  for(uint8_t i = 0; i < n_strips; i++) {
    strips[i].fill_solid(r, g, b);
  }
}

Pixel* StripArray::pixel_at(uint8_t x, uint8_t y) {
  return strips[x].pixel_at(y);
}

void StripArray::noDataDisplay() {
  fill_array(0, 0, 0);
  fill_col(n_strips / 2, 255, 0, 0);
  fill_row(l_strips / 2, 255, 0, 0);
  show();
}

void StripArray::loop() {
  //todo: overflow possible.. do a check...
  if(millis() - last_update > refresh_interval) {
    updateDisplay();
    last_update = millis();
  }
}

void StripArray::setBrightness(uint8_t b) {
  np_strip->setBrightness(b);
}

void StripArray::render() {
  show();
}

uint8_t StripArray::numberOfStrips() {
  return n_strips;
}

uint8_t StripArray::stripLength() {
  return l_strips;
}
void StripArray::begin() {
  char buf[100];
  sprintf(buf, "%d bytes free", System.freeMemory());
  msg->cmprint("We began the strip array /");
  msg->cmprint(buf);
  np_strip->begin();
}

void StripArray::show() {
  np_strip->show();
}

PixelStrip* StripArray::getStrips() {
  return strips;
}
