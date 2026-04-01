#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

// #include <TFT_eSPI.h>
#include <Arduino.h>
#include <globals.h>
#include "LGFX_ILI9488.hpp"

void draw_one_frame();
void draw_menu();
void underline(int x, int y, String word, int font_size);

#endif