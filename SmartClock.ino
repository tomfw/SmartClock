/*
 * Project SmartClock
 * Description:
 * Author:
 * Date:
 */

#pragma SPARK_NO_PREPROCESSOR

#include "Particle.h"
#include "application.h"
#include "pixel_grid/StripArray.h"
#include "neopixel/neopixel.h"
#include "CloudMsg.h"
#include "pixel_grid/WeatherFace.h"
#include "pixel_grid/ClockFace.h"
#include "pixel_grid/TimerFace.h"
//SYSTEM_MODE(AUTOMATIC);

#define FORECAST_INTERVAL_SECS 300
#define FORECAST_HOURS 12

#define MODE_CLOCK 0
#define MODE_WEATHER 1
#define MODE_TIMER 2

#define MAX_ATTEMPTS 5
CloudMsg msg;
int temps[] = {49, 49, 50, 0, 0, 79, 79, 80, 0, 0, 75, 45};
uint8_t probs[] = {0, 0, 0, 0, 0, 0, 0, 0, 39, 40, 75, 100};

Adafruit_NeoPixel strip(144, D2, WS2812B);
//StripArray grid(12, 12, 30, &strip, &msg);
WeatherFace grid(12, 12, 30, &strip, &msg);
ClockFace clockFace(12, 12, 30, &strip, &msg);
TimerFace timerFace(12, 12, 100, &strip, &msg);

int counter = 0;

char data_string[100];
long int last_update = 0;

uint8_t last_pops[12];
int last_temp[12];
bool isOn = true;
uint8_t mode = 1;
char city[30];
char state[10];
int zone = 0;
int timerStart = 0;
int timerEnd = 100;
int weatherAttempts = MAX_ATTEMPTS;
StripArray *currentFace = &timerFace;

void weather_response(const char *event, const char *data);
void tomWeatherHandler(const char *event, const char *data);
void parse_data_str(const char *data);
int setDisplayMode(String mode);
int setTimerHandler(String mode);
int setCityHandler(String city);
int setZoneHandler(String tz);
void publishWeatherRequest();
void saveDefaults();
void readDefaults();
void setDefaults();

void setup() {
  Serial.begin(9600);
  size_t len = EEPROM.length();
  // if(len > 2046) {
  //   setDefaults();
  //   saveDefaults();
  // } else {
     readDefaults();
  // }

  grid.begin(); //this calls strip->begin for all silly silly silly way to do it....
  grid.fill_row(4, 0, 150, 0);
  grid.track_temps(temps, probs);
  grid.show();
  msg.begin();
  Time.zone(zone);
  //Particle.subscribe("hook-response/temp_pop", weather_response, MY_DEVICES);
  Particle.subscribe("hook-response/tomWeather", tomWeatherHandler, MY_DEVICES);
  Particle.variable("weather_data", data_string);
  Particle.variable("timeZone", zone);
  Particle.variable("timerStart", timerStart);
  Particle.variable("timerEnd", timerEnd);
  Particle.variable("mode", mode);
  Particle.variable("city", city);
  Particle.variable("state", state);
  Particle.function("setMode", setDisplayMode);
  Particle.function("setNewTimer", setTimerHandler);
  Particle.function("setCity", setCityHandler);
  Particle.function("setZone", setZoneHandler);
  timerFace.setTimer(timerStart, timerEnd);
  timerFace.resume();
  char buff[16];
  sprintf(buff, " + %d + ", sizeof(city));
  msg.cmprint(buff);
  delay(200);
}

void loop() {
  if(isOn == true) {
    if(mode == MODE_CLOCK) {
      clockFace.loop();
    } else if(mode == MODE_WEATHER) {
      if(weatherAttempts < MAX_ATTEMPTS) {
        grid.loop();
        int elapsed = Time.now() - last_update;
        if(elapsed > FORECAST_INTERVAL_SECS || (elapsed > 30 && weatherAttempts > 1) ) {
         last_update = Time.now();
         publishWeatherRequest();
         weatherAttempts++;
        }
      } else {
        grid.noDataDisplay();
        int elapsed = Time.now() - last_update;
        if (elapsed > 30)  {
          last_update = Time.now();
          publishWeatherRequest();
          weatherAttempts++;
        }
      }
    } else {
      //MODE_TIMER
      timerFace.loop();
    }
  }
  //counter++;
  //char buf[10];
  //sprintf(buf, "Loop: %d / ", counter);
  //msg.cmprint(buf);
  //grid.show();
  //delay(1000);
}

void saveDefaults() {
  int addr = 0;

  EEPROM.put(addr, mode);
  addr++;

  EEPROM.put(addr, timerStart);
  addr += sizeof(int);

  EEPROM.put(addr, timerEnd);
  addr += sizeof(int);

  EEPROM.put(addr, city);
  addr += sizeof(city);

  EEPROM.put(addr, state);
  addr += sizeof(state);

  EEPROM.put(addr, zone);
  addr += sizeof(zone);
}

void readDefaults() {
  int addr = 0;

  int t_start, t_end;
  EEPROM.get(addr, mode);
  addr++;

  EEPROM.get(addr, timerStart);
  addr += sizeof(int);

  EEPROM.get(addr, timerEnd);
  addr += sizeof(int);

  EEPROM.get(addr, city);
  addr += sizeof(city);

  EEPROM.get(addr, state);
  addr += sizeof(state);

  EEPROM.get(addr, zone);
  addr += sizeof(zone);

}

void setDefaults() {
  mode = MODE_CLOCK;
  timerStart = 100;
  timerStart = 0;
  sprintf(city, "%s\0", "Lebanon");
  sprintf(state, "%s\0", "IN");
}

void publishWeatherRequest() {
  char data[256];
  snprintf(data, sizeof(data), "%s~%s", city, state);
  msg.cmprint(data);
  Particle.publish("tomWeather", data, PRIVATE);
}

int setDisplayMode(String m) {
  if(m.equals("off")) {
      currentFace->fill_array(0, 0, 0);
      currentFace->show();
      isOn = false;
  } else if (m.equals("timer")) {
    timerFace.resume();
    isOn = true;
    mode = MODE_TIMER;
    currentFace = &timerFace;
  } else if (m.equals("clock")) {
    isOn = true;
    mode = MODE_CLOCK;
    currentFace = &clockFace;
  } else if (m.equals("weather")) {
    isOn = true;
    mode = MODE_WEATHER;
    currentFace = &grid;
  }
  saveDefaults();
  return 11;
}

int setTimerHandler(String timer) {
  //parse the string and call set timer...
  char timerString[32];
  timer.getBytes((byte*)timerString, 32);
  unsigned long start, end;
  sscanf(timerString, "%lu~%lu", &start, &end);
  timerFace.setTimer(start, end);
  timerFace.resume();
  timerStart = (int) start;
  timerEnd = (int)end;
  saveDefaults();

}

int setCityHandler(String loc_str) {
  char location[100];
  loc_str.getBytes((byte*)location, 100);
  char *city_a = strtok(location, "~");
  char *state_a = strtok(NULL, "~");
  sprintf(city, "%s\0", city_a);
  sprintf(state, "%s\0", state_a);

  publishWeatherRequest(); //try to get new data
  weatherAttempts = MAX_ATTEMPTS; //manually invalidate displayed data

  saveDefaults();
}

int setZoneHandler(String tz) {
  char tz_a[10];
  tz.getBytes((byte*)tz_a, 10);
  zone = atoi(tz_a);
  Time.zone(zone);
  saveDefaults();
  return 0;
}

void weather_response(const char *event, const char *data) {
    sprintf(data_string, "event: %s, data: %s", event, data);
    parse_data_str(data);
    for(uint8_t i = 0; i < FORECAST_HOURS; i++) {
      temps[i] = last_temp[i];
      probs[i] = last_pops[i];
    }
    weatherAttempts = 0;
}

void tomWeatherHandler(const char *event, const char *data) {
    sprintf(data_string, "event: %s, data: %s", event, data);
    parse_data_str(data);
    for(uint8_t i = 0; i < FORECAST_HOURS; i++) {
      temps[i] = last_temp[i];
      probs[i] = last_pops[i];
    }
    weatherAttempts = 0;
}

void parse_data_str(const char *data) {
    sscanf(data,"%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d~%d",
    &last_pops[0], &last_temp[0],
    &last_pops[1], &last_temp[1],
    &last_pops[2], &last_temp[2],
    &last_pops[3], &last_temp[3],
    &last_pops[4], &last_temp[4],
    &last_pops[5], &last_temp[5],
    &last_pops[6], &last_temp[6],
    &last_pops[7], &last_temp[7],
    &last_pops[8], &last_temp[8],
    &last_pops[9], &last_temp[9],
    &last_pops[10], &last_temp[10],
    &last_pops[11], &last_temp[11] );
}
