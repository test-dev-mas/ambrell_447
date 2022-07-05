#include <TouchScreen.h>
#include <avr/wdt.h>

#include "essentials.hpp"
#include "ui.hpp"

#define YP A1       // must be an analog pin, use "An" notation!
#define XM A0       // must be an analog pin, use "An" notation!
#define YM 14       // can be a digital pin
#define XP 15       // can be a digital pin

#define TS_MIN_X 920
#define TS_MAX_X 120

#define TS_MIN_Y 940
#define TS_MAX_Y 69

#define MINPRESSURE 10
#define MAXPRESSURE 1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void touch(Adafruit_TFTLCD &tft, uint16_t x_0, uint16_t x_1, uint16_t y_0, uint16_t y_1) {
    while (1) {
        TSPoint p =ts.getPoint();
        pinMode(XM,OUTPUT);
        pinMode(YP,OUTPUT);
        if (p.z>MINPRESSURE && p.z<MAXPRESSURE) {
            p.x = map(p.x, TS_MIN_X, TS_MAX_X, tft.width(), 0);
            p.y = map(p.y, TS_MIN_Y, TS_MAX_Y, tft.height(), 0);

            if ((p.x>x_0 && p.x<x_1) && (p.y>y_0 && p.y <y_1)) {
                break;
            }  
        }
    }
}

int touch(Adafruit_TFTLCD &tft, uint16_t x_0, uint16_t x_1, uint16_t y_0, uint16_t y_1, uint16_t x_2, uint16_t x_3, uint16_t y_2, uint16_t y_3) {
    while (1) {
        TSPoint p =ts.getPoint();
        pinMode(XM,OUTPUT);
        pinMode(YP,OUTPUT);
        if (p.z>MINPRESSURE && p.z<MAXPRESSURE) {
            p.x = map(p.x, TS_MIN_X, TS_MAX_X, tft.width(), 0);
            p.y = map(p.y, TS_MIN_Y, TS_MAX_Y, tft.height(), 0);

            if ((p.x>x_0 && p.x<x_1) && (p.y>y_0 && p.y <y_1)) {
                return 0;
            }
            else if ((p.x>x_2 && p.x<x_3) && (p.y>y_2 && p.y <y_3)) {
                return 1;
            }
        }
    }
}

/* same as touch(), but with a timeout parameter */
int touch(Adafruit_TFTLCD &tft, uint16_t x_0, uint16_t x_1, uint16_t y_0, uint16_t y_1, uint16_t x_2, uint16_t x_3, uint16_t y_2, uint16_t y_3, uint32_t timeout) {
    uint32_t t_s = millis();
    while (millis() - t_s < timeout) {
        TSPoint p =ts.getPoint();
        pinMode(XM,OUTPUT);
        pinMode(YP,OUTPUT);
        if (p.z>MINPRESSURE && p.z<MAXPRESSURE) {
            p.x = map(p.x, TS_MIN_X, TS_MAX_X, tft.width(), 0);
            p.y = map(p.y, TS_MIN_Y, TS_MAX_Y, tft.height(), 0);

            if ((p.x>x_0 && p.x<x_1) && (p.y>y_0 && p.y <y_1)) {
                return 0;
            }
            else if ((p.x>x_2 && p.x<x_3) && (p.y>y_2 && p.y <y_3)) {
                return 1;
            }
        }
    }
    return 1;
}

/* a special function for debug mode */
int touch9(Adafruit_TFTLCD &tft) {
    while (1) {
        TSPoint p =ts.getPoint();
        pinMode(XM,OUTPUT);
        pinMode(YP,OUTPUT);
        if (p.z>MINPRESSURE && p.z<MAXPRESSURE && p.x > 10) {
            p.x = map(p.x, TS_MIN_X, TS_MAX_X, tft.width(), 0);
            p.y = map(p.y, TS_MIN_Y, TS_MAX_Y, tft.height(), 0);
            if ((p.x>110 && p.x<210) && (p.y>438 && p.y <476)) {
                return 0;
            }
            else if ((p.y>60 && p.y <106)) {
                return 1;
            }
            else if ((p.y>107 && p.y <153)) {
                return 2;
            }
            else if ((p.y>154 && p.y <200)) {
                return 3;
            }
            else if ((p.y>201 && p.y <247)) {
                return 4;
            }
            else if ((p.y>248 && p.y <294)) {
                return 5;
            }
            else if ((p.y>295 && p.y <341)) {
                return 6;
            }
            else if ((p.y>342 && p.y <388)) {
                return 7;
            }
            else if ((p.y>389 && p.y <435)) {
                return 8;
            }
        }
    }
}

void comm_lost(Adafruit_TFTLCD &tft) {
    tft.fillRect(40, 160, 240, 240, 0xC5F7);
    tft.setCursor(55,205);
    tft.setTextColor(color_warning);
    tft.setTextSize(1);
    tft.print("Communication lost!");

    tft.fillRoundRect(80, 281, 160, 40, 8, color_warning);
    tft.setCursor(130,305);
    tft.setTextColor(0xffff);
    tft.setTextSize(1);
    tft.print("RESET");

    touch(tft, 80, 240, 281, 321);
    wdt_enable(WDTO_15MS);
    while (1) {}
}