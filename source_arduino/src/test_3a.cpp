#include "essentials.hpp"
//
#include "ui.hpp"
#include "test_3a.hpp"

static bool test_pass = true;

bool test_3a(Adafruit_TFTLCD &tft, char* test_name) {
    tft.fillScreen(0x2104);

    tft.fillRect(0, 0, 320, 60, color_primary);
    tft.setCursor(30, 30);
    tft.setTextColor(0xffff);
    tft.println(test_name);

    Serial.write(0x42);                         // START command
    uint32_t t_s = millis();
    for (;;) {
        if (Serial.available()) {
            uint8_t u = Serial.read();
            if (u == 0x2B) {                    // we rely on DUT to set pass/fail condition
                tft.setCursor(60, 220);
                tft.setTextColor(color_pass);
                tft.println("RS485 loopback pass");
                break;
            }
            else if (u == 0x2C) {               // either a timeout or wrong byte will trigger fail signal
                test_pass &= false;
                tft.setCursor(60, 220);
                tft.setTextColor(color_fail);
                tft.println("RS485 loopback fail");
                break;
            }
        }
        if (millis()-t_s>=2000) {
                comm_lost(tft);
            }
    }

    if (test_pass) {
        /* draw an arrow */
        tft.fillCircle(160, 430, 25, color_primary);
        tft.drawLine(145, 430, 175, 430, 0xffff);
        tft.drawLine(160, 420, 175, 430, 0xffff);
        tft.drawLine(160, 440, 175, 430, 0xffff);

        tft.setCursor(30, 430);
        tft.setTextColor(0x07e0);
        tft.setTextSize(2);
        tft.println("PASS");
        tft.setTextSize(1);
    }
    else {
        /* draw a cross */
        tft.fillCircle(160, 430, 25, color_warning);
        tft.drawLine(145, 415, 175, 445, 0xffff);
        tft.drawLine(145, 445, 175, 415, 0xffff);
    }
    /* requires user input */
    //touch(tft, 135, 185, 405, 455); //
delay(1000);
    return test_pass;
}