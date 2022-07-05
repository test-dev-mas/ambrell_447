#ifndef DEBUG_MODE_H
#define DEBUG_MODE_H

#include <Adafruit_GFX.h>       // Core graphics library
#include <Adafruit_TFTLCD.h>    // Hardware-specific library

void debug_mode(Adafruit_TFTLCD &tft, char* test_name);

#endif