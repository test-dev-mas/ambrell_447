#include "essentials.hpp"
//
#include "ui.hpp"
#include "test_2a.hpp"

static bool test_pass = true;

bool test_2a(Adafruit_TFTLCD &tft, char* test_name) {
    tft.fillScreen(0x2104);

    tft.fillRect(0, 0, 320, 60, color_primary);
    tft.setCursor(30, 30);
    tft.setTextColor(0xffff);
    tft.println(test_name);

    tft.setCursor(0, 100);
    tft.println("J11.1 (FLS1)");
    /* in its initialized state, J11.1 is pulled high to 5V
        PH4 on DUT is in high-z
        A2 should read 1
    */
    if (digitalRead(A2)) {
        tft.setCursor(160, 120);
        tft.setTextColor(color_pass);
        tft.println("contact open");
    }
    else {
        test_pass &= false;
        tft.setCursor(160, 120);
        tft.setTextColor(color_fail);
        tft.println("contact close");  
    }
    /* when activated, PH4 on DUT should source current
        A2 should read 0
    */
    Serial.write(0x30);                         // START command
    uint32_t t_s = millis();
    for (;;) {
        if (Serial.available()) {
            uint8_t u = Serial.read();
            if (u == 0x03) {                    // ACK from DUT
                if (digitalRead(A2)) {          // DUT is supposed to pull PH4 low
                    tft.setCursor(160, 160);
                    tft.setTextColor(color_fail);
                    tft.println("contact open");
                    test_pass &= false;
                    break;
                }
                else {
                    tft.setCursor(160, 160);
                    tft.setTextColor(color_pass);
                    tft.println("contact close");
                    break;
                }
            }
        }
        if (millis()-t_s>=2000) {
            comm_lost(tft);
        }
    }

    tft.setCursor(0, 220);
    tft.setTextColor(0xffff);
    tft.println("J12.1 (FLS2)");
    /* in its initialized state, J12.1 is pulled high to 5V
        PH5 on DUT is in high-z
        A3 should read 1
    */
    if (digitalRead(A3)) {
        tft.setCursor(160, 240);
        tft.setTextColor(color_pass);
        tft.println("contact open");
    }
    else {
        test_pass &= false;
        tft.setCursor(160, 240);
        tft.setTextColor(color_fail);
        tft.println("contact close");
    }
    /* when activated, PH5 on DUT should source current
        A3 should read 0
    */
    Serial.write(0x31);                          // START command
    t_s = millis();
    for (;;) {
        if (Serial.available()) {
            uint8_t u = Serial.read();
            if (u == 0x03) {                    // ACK from DUT
                if (digitalRead(A3)) {          // DUT is supposed to pull PH5 low
                    tft.setCursor(160, 280);
                    tft.setTextColor(color_fail);
                    tft.println("contact open");
                    test_pass &= false;
                    break;
                }
                else {
                    tft.setCursor(160, 280);
                    tft.setTextColor(color_pass);
                    tft.println("contact close");
                    break;
                }
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