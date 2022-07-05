#include "essentials.hpp"
//
#include "ui.hpp"
#include "test_3d.hpp"

static bool test_pass = true;

bool test_3d(Adafruit_TFTLCD &tft, char* test_name) {
    tft.fillScreen(0x2104);

    tft.fillRect(0, 0, 320, 60, color_primary);
    tft.setCursor(30, 30);
    tft.setTextColor(0xffff);
    tft.println(test_name);

    Serial.write(0x45);                         // START command
    uint32_t t_s = millis();
    for (;;) {
        if (Serial.available()) {
            uint8_t u = Serial.read();
            if (u == 0x2B) {                            
                tft.setCursor(60, 100);
                tft.setTextColor(color_pass);
                tft.println("Display loopback pass");
                break;
            }
            else if (u == 0x2C) {                       //
                test_pass &= false;
                tft.setCursor(60, 100);
                tft.setTextColor(color_fail);
                tft.println("Display loopback fail");
                break;
            }
        }           
        if (millis()-t_s>=2000) {
            comm_lost(tft);
        }
    }

    /* display test pattern, this part should nested inside this if statement */
    if (test_pass) {
        Serial.write(0x46);                                         // ask DUT to draw test pattern

        /* ask operator to confirm test pattern on external display */
        tft.fillRect(40, 160, 240, 200, 0xC5F7);
        tft.setCursor(55,190);
        tft.setTextColor(0x0000);
        tft.setTextSize(1);
        tft.print("Is there a");

        tft.setCursor(55,210);
        tft.setTextColor(0x0000);
        tft.setTextSize(1);
        tft.print("test pattern");

        tft.setCursor(55,230);
        tft.setTextColor(0x0000);
        tft.setTextSize(1);
        tft.print("on external display?");

        tft.drawLine(41, 300, 279, 300, 0xffff);

        tft.setCursor(90,340);
        tft.setTextColor(color_primary);
        tft.setTextSize(1);
        tft.print("YES");

        tft.setCursor(220,340);
        tft.setTextColor(color_primary);
        tft.setTextSize(1);
        tft.print("NO");

        /* a timeout is added to prevent from powering DUT for too long */
        if (touch(tft, 40, 159, 301, 360, 160, 399, 301, 360, 100000)) {
            tft.fillRect(40, 160, 240, 200, 0x2104);                // cover up message box
            test_pass &= false;
            tft.setCursor(55, 210);
            tft.setTextColor(color_fail);
            tft.println("NO input. Time out!");
            // break;
        }
        else {
            tft.fillRect(40, 160, 240, 200, 0x2104);                // cover up message box
            // tft.setCursor(60, 220);
            // tft.setTextColor(color_pass);
            // tft.println("Display test pass");
            // break;
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