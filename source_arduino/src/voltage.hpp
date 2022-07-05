#ifndef MEASURE_H
#define MEASURE_H

#include <Adafruit_TFTLCD.h>

int read_adc();
void init_adc();
void setup_relay(uint8_t pins);
bool measure(Adafruit_TFTLCD &tft);

#endif