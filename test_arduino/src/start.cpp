#include "essentials.hpp"
#include "start.hpp"
#include "ui.hpp"

int start(Adafruit_TFTLCD &tft) {
    // tft.fillScreen(0x0000);
    tft.fillScreen(color_primary);
    tft.setCursor(40,50);
    tft.setTextColor(0xffff);
    tft.setTextSize(3);
    tft.setFont(&FreeMonoBold9pt7b);
    tft.print("AMBRELL");

    tft.setCursor(50,80);
    tft.setTextColor(0xffff);
    tft.setTextSize(1);
    tft.print("DISPLAY/SERVER BOARD");

    tft.setCursor(120,200);
    tft.setTextColor(color_warning);
    tft.setTextSize(1);
    tft.print("WARNING");

    tft.setCursor(40,220);
    tft.setTextColor(0xffff);
    tft.setTextSize(1);
    // tft.setFont(&FreeMono9pt7b);
    tft.print("Upload firmware to DUT");
    tft.setCursor(50,240);
    tft.print("before proceeding.");

    tft.drawRoundRect(80, 261, 160, 40, 8, 0xffff);
    tft.setCursor(120,285);
    tft.setTextColor(0xffff);
    tft.setTextSize(1);
    // tft.setFont(&FreeMono9pt7b);
    tft.print("Confirm");


    // tft.fillRect(0,380,320,100,0x0000);

    tft.setCursor(0,430);
    tft.setTextColor(color_bg_l);
    tft.setTextSize(1);
    tft.print("Microart Services INC 2022");

    /* debug mode */
    tft.fillRoundRect(220, 440, 100, 40, 8, 0x632C);
    tft.setCursor(247,463);
    tft.setTextColor(0xCE59);
    tft.setTextSize(1);
    tft.print("DEBUG");

    return touch(tft, 80, 240, 261, 301, 220, 320, 440, 480);
}