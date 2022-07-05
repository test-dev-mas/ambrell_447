#ifndef ESSENTIALS_H
#define ESSENTIALS_H

#include <Adafruit_GFX.h>           // Core graphics library
#include <Adafruit_TFTLCD.h>        // Hardware-specific library
#include <Fonts/FreeMonoBold9pt7b.h>

#define color_primary   0x70F3
#define color_dark      0x400D
#define color_light     0xAA99
#define color_bg        0xDEFB
#define color_bg_l      0xEF9D
#define color_warning   0xF8A8
#define color_pass      0x66c2
#define color_fail      0xF8A8

/* in the format of arduino pin:D2/3/4/5/6/7/8/9 -> hc154:U1/U2/U3/U4/A0/A1/A2/A3 */
#define RT1             0b01110000
#define RT2             0b01111000
#define RT3             0b01110100
#define RT4             0b01111100
#define RT5             0b01110010
#define RT6             0b01111010
#define RT7             0b01110110
#define RT8             0b01111110
#define RT17            0b10110000
#define RT18            0b10111000
#define RT19            0b10110100
#define RT20            0b10111100
#define RT21            0b10110010
#define RT22            0b10111010
#define RT23            0b10110110
#define RT24            0b10111110

#define RT67_ON         pinMode(35, OUTPUT);digitalWrite(35, HIGH);
#define RT67_OFF        pinMode(35, OUTPUT);digitalWrite(35, LOW);

// char* test_names[] = {"Voltage Measurement", "Contact Open & Close", "Key Press Test", "LED Test", "RS485 Test", "USB Test", "SD Test", "Display Test"};

#endif