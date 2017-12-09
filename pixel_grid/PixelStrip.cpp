#include "PixelStrip.h"
#include <math.h>

#define EMPTY_SPOT 255

PixelStrip::PixelStrip( Adafruit_NeoPixel* s, uint8_t i, uint8_t len, bool reversed, CloudMsg *m) {
  np_strip = s;
  i_start = i;
  n_pixels = len;
  n_spots = 5;
  animation_delay = 10;
  i_spots = new uint8_t[n_spots];
  for(uint8_t i = 0; i < n_spots; i++) {
    i_spots[i] = EMPTY_SPOT;
  }
  isReversed = reversed;
  msg = m;
  pixels = new Pixel[len];
  uint8_t idx;
  for(uint8_t j = 0; j < n_pixels; j++) {
    idx = i_start + j;
    if(isReversed) {
      idx = (i_start + (n_pixels - 1) - j);
    }
    pixels[j] = Pixel(np_strip, idx, msg);
  }
}

void PixelStrip::set_animation_delay(uint8_t d) {
  animation_delay = d;
}

void PixelStrip::set_n_spots(uint8_t n) {
  n_spots = n;
}

void PixelStrip::render() {
  if(frames > animation_delay) {
    frames = 0;
    if(n_spots == 0) return;
    uint8_t min_index = n_pixels;
    uint8_t space = floor(n_pixels / n_spots);
    for(uint8_t spot = 0; spot < n_spots; spot++) {
      if(i_spots[spot] != EMPTY_SPOT) {
        i_spots[spot] += 1;
        if(i_spots[spot] == n_pixels) i_spots[spot] = EMPTY_SPOT;
      } else {
        min_index = n_pixels;
        for(uint8_t j = 0; j < n_spots; j++)
          min_index = min_index < i_spots[j] ? min_index : i_spots[j];
        if(min_index >= space) {
          i_spots[spot] = 0;
        }
      }
    }
  }

  for(uint8_t spot = 0; spot < n_spots; spot++) {
    if(i_spots[spot] != EMPTY_SPOT)
      pixels[i_spots[spot]].set(0, 0, 45);
  }
  frames++;
}

void PixelStrip::gradient(uint8_t start, uint8_t end, uint8_t *c_start, uint8_t *c_end) {
  uint8_t len = end - start;
  uint8_t color[3];

  for(uint8_t i = 0; i < len; i++) {
    uint8_t j = len - i - 1;
    color[0] = ((c_start[0] * (j)) + (c_end[0] * i)) / len;
    color[1] = ((c_start[1] * (j)) + (c_end[1] * i)) / len;
    color[2] = ((c_start[2] * (j)) + (c_end[2] * i)) / len;
    pixels[i + start].set(color[0], color[1], color[2]);
  }
  free(c_start);
  free(c_end);
}

Pixel* PixelStrip::pixel_at(uint8_t index) {
  return &pixels[index];
}

void PixelStrip::clear(uint8_t start, uint8_t end) {
  for(uint8_t i = start; i < end; i++) {
    pixels[i].set(0, 0, 0);
  }
}

void PixelStrip::fill_solid(uint8_t r, uint8_t g, uint8_t b) {
  for(int i = 0; i < n_pixels; i++) {
    pixels[i].set(r, g, b);
  }
}

uint8_t *PixelStrip::get_pixel(uint8_t index) {

}

uint8_t *PixelStrip::get_ordered_pixel(uint8_t index) {

}

void PixelStrip::set_pixel(uint8_t index, uint8_t *pixel) {

}

int PixelStrip::convert_index(uint8_t index) {
  if(isReversed) {
    return (n_pixels - index) * 3;
  }
  return index * 3;
}

PixelStrip::~PixelStrip() {
}
