#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
// #include <TFT_eSPI.h>
#include <vector>
#include <ESP32Encoder.h>
#include "LGFX_ILI9488.hpp"
#include <time.h>

extern LGFX lcd;
extern LGFX_Sprite canvas;
extern LGFX_Sprite canvas1;
extern LGFX_Sprite canvas2;

extern ESP32Encoder encoder;

// menu
extern String current_tab;
extern int selected;
extern std::vector<String> menu_tabs;

// encoder
extern long last_step;
extern char state_enc;

extern bool in_menu;

extern hw_timer_t * timer;

extern int current_time[6];
extern const char* ntp_server;
extern int counter_update;

extern float current_temp = 0.0;
extern const char* api_key_temp;
extern const char* city;

#endif