#include "TimerFace.h"

void TimerFace::updateDisplay() {
  if(running) {
    if(Time.now() < end_time) {
      uint8_t total_pixels = numberOfStrips() * stripLength();
      double pct = (double)(Time.now() - start_time) / (double)(end_time - start_time);
      uint8_t target_pixels = (uint8_t)(pct * total_pixels);
      addPixels(target_pixels - n_pixels);
    } else {
      fill_array(255, 0, 0);
      setBrightness(done_brightness);
      if(done_ascending) {
        done_brightness+=3;
        if(done_brightness > 254) {
          done_ascending = false;
          done_brightness = 255;
        }
      } else {
        done_brightness-=3;
        if(done_brightness < 50) {
          done_ascending = true;
          //done_brightness = 50;
        }
      }
    }
  }
  if(start_time == 0) {
    fill_array(0, 0, 0);
  }
  render();
}

void TimerFace::resume() {
  if(Time.now() < end_time) {
    fill_array(0, 0, 0);
    show();
    n_pixels = 0;
    last_row = 0;
  } else {
    fill_array(255, 0, 0);

  }
}

void TimerFace::addPixels(uint8_t n) {
  uint8_t pixels_added = 0;
  uint8_t total_pixels = numberOfStrips() * stripLength();
  Pixel *pix;
  while(pixels_added < n && n_pixels < total_pixels) {
    int free_x = -1;
    uint8_t frees_found = 0;
    for(int i = 0; i < numberOfStrips(); i++) {
      pix = pixel_at(i, last_row);
      if(!pix->isOn()) {
        frees_found++;
        free_x = i;
      }
    }
    if(frees_found == 0) {
      last_row++;
    } else if(frees_found == 1) {
      pix = pixel_at(free_x, last_row);
      pix->set(100, 100, 150);
      n_pixels++;
      pixels_added++;
      last_row++;
    } else {
      pix = pixel_at(random(numberOfStrips()), last_row);
      while(pix->isOn())
        pix = pixel_at(random(numberOfStrips()), last_row);
      n_pixels++;
      pixels_added++;
      pix->set(100, 100, 150);
    }
  }
}

TimerFace::TimerFace(int n, int l, int refresh_rate, Adafruit_NeoPixel *strip, CloudMsg *cm)
  : StripArray(n, l, refresh_rate, strip, cm) {
    done_brightness = 255;
    done_ascending = false;
}

void TimerFace::setTimer(int s, int e) {
  start_time = s;
  end_time = e;
  fill_array(0, 0, 0);
  running=true;
  if(s == 0) {
    running = false;
    render();
  }
}
