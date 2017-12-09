#include "ClockFace.h"
#include "PixelStrip.h"
#include "math.h"

#define PI 3.14159

ClockFace::ClockFace(int n, int l, int refresh_rate, Adafruit_NeoPixel *strip, CloudMsg *cm)
  : StripArray(n, l, refresh_rate, strip, cm) {

    //grid.show();
    //delay(1000);
    havePrinted = false;
}

void ClockFace::updateDisplay() {
  if(!havePrinted) {
    updateTime();
    char buf[30];
    sprintf(buf, "%d:%d:%d ", hours, minutes, seconds);
    msg->cmprint(buf);
    havePrinted=true;
  }
  fill_array(0, 0, 0);
  updateTime();
  //drawFace(0, 0, 100);
  drawTime();
  render();
}

void ClockFace::drawTime() {

  drawHand(x_hour(hours), y_hour(hours), 200, 200, 200);
  drawHand(x_minute(minutes), y_minute(minutes), 200, 0, 0);
  drawHand(x_minute(seconds), y_minute(seconds), 0, 200, 0);
}

void ClockFace::drawHand(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  Pixel *outPix, *inPix;
  int xOff, yOff;

  if(x==0) {
    xOff = 1;
    yOff = 0;//y == 0 ? 1 : 0;
    yOff = 0;//y == stripLength() - 1 ? -1 : yOff;

  } else if(x == numberOfStrips() - 1) {
    xOff = -1;
    yOff = y == 0 ? 1 : 0;
    yOff = 0;//y == stripLength() - 1 ? -1 : yOff;
  }

  if(y == 0) {
    yOff = 0;//1;
  } else if (y == stripLength() - 1) {
    yOff = 0;//-1;
  }
  outPix = pixel_at(x, y);
  //inPix = pixel_at(x + xOff, y + yOff);

  outPix->set(r, g, b);
  //inPix->set(r, g, b);
}

uint8_t ClockFace::x_minute(uint8_t min) {
  //map a number (0-59) to clock face
  uint8_t x = 0;
  if(min > 5 && min < 25)
    return numberOfStrips() - 1;
  if(min > 35 && min < 55)
    return 0;

  if(min >= 55 || min <= 5) { //map to top
    if(min > 54) { //0 to 6
      x = map(min, 55, 59, 0, numberOfStrips() / 2);
    } else if(min <= 5) {
      x = map(min, 0, 5, numberOfStrips() / 2, numberOfStrips() - 1);
    }
  } else { //map to bottom
    x = map(min, 25, 35, numberOfStrips() - 1, 0);
  }
  return x;
}

uint8_t ClockFace::x_hour(uint8_t hour) {
  uint8_t x = 0;
  if(hour > 1 && hour < 5)
    return numberOfStrips() - 1;
  if(hour > 7 && hour < 11)
    return 0;

  /*if(hour <= 12 && hour >= 11) { //hour 11..12
    x = map(hour, 11, 12, 0, numberOfStrips() / 2);
  }*/
  if(hour == 11) {
    x = 0;
  } else if(hour==12 || hour == 0) {
    x = numberOfStrips() / 2;
  } else if(hour == 1) {
    x = numberOfStrips() - 1;
  } else {
    x = map(hour, 5, 7, numberOfStrips() - 1, 0);
  }
  return x;
}

uint8_t ClockFace::y_minute(uint8_t min) {
  uint8_t y;
  if(min >= 55 || min <= 5)
    return stripLength() - 1;
  if(min >= 25 && min <= 35)
    return 0;

  if(min > 5 && min < 25) {
    y = map(min, 6, 24, stripLength() - 1, 0);
  } else {
    y = map(min, 36, 54, 0, stripLength() - 1);
  }
  return y;
}

uint8_t ClockFace::y_hour(uint8_t hour) {
  uint8_t y = 0;
  if(hour >= 11 || hour <= 1)
    return stripLength() - 1;
  if(hour >= 5 && hour <= 7)
    return 0;

  if(hour > 1 && hour < 5) {
    y = map(hour, 1, 5, stripLength() - 1, 0);
  } else {
    y = map(hour, 7, 11, 0, stripLength() - 1);
  }
  return y;
}

void ClockFace::drawFace(uint8_t r, uint8_t g, uint8_t b) {
  fill_col(0, r, g, b);
  fill_col(1, r, g, b);
  fill_col(numberOfStrips() - 1, r, g, b);
  fill_col(numberOfStrips() - 2, r, g, b);

  fill_row(0, r, g, b);
  fill_row(1, r, g, b);
  fill_row(stripLength() - 1, r, g, b);
  fill_row(stripLength() - 2, r, g, b);
}

void ClockFace::traceTo(uint8_t target_x, uint8_t target_y, uint8_t r, uint8_t g, uint8_t b) {
  uint8_t x = numberOfStrips() / 2 - 1;
  uint8_t y = stripLength() - 1;
  Pixel *pix = pixel_at(x, y);
  pix->set(r, g, b);

  bool done = false;
  bool doneDone = false;


  // across the top
  while(!done) {
    x += 1;
    if(x == numberOfStrips() - 1) {
      done = true;
    }
    if(x == target_x && y == target_y) {
      doneDone = true;
      done = true;
    }
    pix = pixel_at(x, y);
    pix->set(r, g, b);
  }
  if (doneDone) return;
  done = false;

  //down the right
  while(!done) {
    y -= 1;
    if(y == 0) {
      done = true;
    }
    if(x == target_x && y == target_y) {
      doneDone = true;
      done = true;
    }
    pix = pixel_at(x, y);
    pix->set(r, g, b);
  }
  if (doneDone) return;
  done = false;

  //across the bottom
  while(!done) {
    x -= 1;
    if(x == 0) {
      done = true;
    }
    if(x == target_x && y == target_y) {
      doneDone = true;
      done = true;
    }
    pix = pixel_at(x, y);
    pix->set(r, g, b);
  }
  if (doneDone) return;
  done = false;

  //up the left
  while(!done) {
    y += 1;
    if(y == stripLength() - 1) {
      done = true;
    }
    if(x == target_x && y == target_y) {
      doneDone = true;
      done = true;
    }
    pix = pixel_at(x, y);
    pix->set(r, g, b);
  }
  if (doneDone) return;
  done = false;

  //across the top
  while(!done) {
    x += 1;
    if(x == numberOfStrips() / 2 - 1) {
      done = true;
    }
    if(x == target_x && y == target_y) {
      doneDone = true;
      done = true;
    }
    pix = pixel_at(x, y);
    pix->set(r, g, b);
  }
}

void ClockFace::updateTime() {
  hours = Time.hourFormat12();
  minutes = Time.minute();
  seconds = Time.second();
  isPM = (Time.isPM(Time.local())) == true;
}
