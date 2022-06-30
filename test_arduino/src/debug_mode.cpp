#include <avr/wdt.h>

#include "essentials.hpp"
#include "ui.hpp"
#include "debug_mode.hpp"

#include "voltage.hpp"
#include "start.hpp"
#include "test_2a.hpp"
#include "test_2b.hpp"
#include "test_2c.hpp"
#include "test_3a.hpp"
#include "test_3b.hpp"
#include "test_3c.hpp"
#include "test_3d.hpp"

char* test_names[] = {"Voltage Measurement", "Contact Open & Close", "Key Press Test", "LED Test", "RS485 Test", "USB Test", "SD Test", "Display Test"};

void debug_mode(Adafruit_TFTLCD &tft, char* test_name) {
    tft.fillScreen(0x2104);
    
    tft.fillRect(0, 0, 320, 60, color_primary);
    tft.setCursor(30, 30);
    tft.setTextColor(0xffff);
    tft.println(test_name);

    for (auto i=0;i<8;i++) {
        tft.fillRect(0, 61+i*47, 320, 46, 0x632C);
        tft.setCursor(30, 85+i*47);
        tft.setTextColor(0xCE59);
        tft.print(test_names[i]);
    }

    /* reset button */
    tft.fillRoundRect(110, 438, 100, 38, 8, color_warning);
    tft.setCursor(135,463);
    tft.setTextColor(0xffff);
    tft.setTextSize(1);
    tft.print("RESET");

    // int a = touch9(tft);
    // Serial.println(a);
    switch (touch9(tft))
    {
    case 0:
        wdt_enable(WDTO_15MS);
        while (1) {}
        break;
    case 1:
        measure(tft);
        break;
    case 2:
        test_2a(tft, test_names[1]);
        break;
    case 3:
        test_2b(tft, test_names[2]);
        break;
    case 4:
        test_2c(tft, test_names[3]);
        break;
    case 5:
        test_3a(tft, test_names[4]);
        break;
    case 6:
        test_3b(tft, test_names[5]);
        break;
    case 7:
        test_3c(tft, test_names[6]);
        break;
    case 8:
        test_3d(tft, test_names[7]);
        break;
    
    default:
        break;
    }
}