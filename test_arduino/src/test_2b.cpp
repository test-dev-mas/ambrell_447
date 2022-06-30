#include "essentials.hpp"
//
#include "ui.hpp"
#include "test_2b.hpp"

static bool test_pass = true;
uint8_t keys[] = {A8, A9, A10, A11, A12, A13, A14, A15, 32, 33};
char* key_names[] = {"KP8", "KP7", "KP6", "KP5", "KP4", "KP3", "UAR", "KP2", "KP1", "RAR"};

bool test_2b(Adafruit_TFTLCD &tft, char* test_name) {
    tft.fillScreen(0x2104);

    tft.fillRect(0, 0, 320, 60, color_primary);
    tft.setCursor(30, 30);
    tft.setTextColor(0xffff);
    tft.println(test_name);

    uint16_t row = 80;
    /* each key is tested in this for-loop */
    for (auto i=0;i<sizeof(keys)/sizeof(keys[0]);i++) {
        tft.setCursor(0, row);
        tft.setTextColor(0xffff);
        tft.println(key_names[i]);

        /* i added to base command 0x32, see DUT source code for details */
        Serial.write(0x32+i);                   // start test signal
        uint32_t t_s = millis();
        for (;;) {
            if (Serial.available()) {
                uint8_t u = Serial.read();
                if (u == 0x2B) {
                    tft.setCursor(20, row+16);
                    tft.setTextColor(color_pass);
                    tft.println("key up");
                    break;
                }
                else if (u == 0x2C) {
                    tft.setCursor(20, row+16);
                    tft.setTextColor(color_fail);
                    tft.println("key down");
                    test_pass &= false;
                    break;
                }
            }
            if (millis()-t_s>=2000) {
                comm_lost(tft);
            }
        }

        pinMode(keys[i], OUTPUT);               // simulate a key press
        digitalWrite(keys[i], LOW);

        Serial.write(0x32+i);                   // repeat first step, but reverse logic
        t_s = millis();
        for (;;) {
            if (Serial.available()) {
                uint8_t u = Serial.read();
                if (u == 0x2C) {
                    tft.setCursor(180, row+16);
                    tft.setTextColor(color_pass);
                    tft.println("key down");
                    break;
                }
                else if (u == 0x2B) {
                    tft.setCursor(180, row+16);
                    tft.setTextColor(color_fail);
                    tft.println("key up");
                    test_pass &= false;
                    break;
                }
            }
            if (millis()-t_s>=2000) {
                comm_lost(tft);
            }
        }
        digitalWrite(keys[i], HIGH);            // stop current from flowing on keys
        row += 32;
    }

    /* any failure should bring test_pass to 'false' */
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