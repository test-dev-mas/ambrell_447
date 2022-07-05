#include "essentials.hpp"
//
#include "ui.hpp"
#include "test_2c.hpp"

static bool test_pass = true;
bool led_pass[] = {true, true, true, true};
uint8_t led_test_points[] = {A7, A6, A5, A4};
char* led_names[] = {"FAULT", "LIMIT", "HEAT", "READY"};

bool test_2c(Adafruit_TFTLCD &tft, char* test_name) {
    tft.fillScreen(0x2104);

    tft.fillRect(0, 0, 320, 60, color_primary);
    tft.setCursor(30, 30);
    tft.setTextColor(0xffff);
    tft.println(test_name);

    uint16_t row = 80;
    for (auto i=0;i<sizeof(led_test_points)/sizeof(led_test_points[0]);i++) {
        tft.setCursor(0, row);
        tft.setTextColor(0xffff);
        tft.println(led_names[i]);

        tft.drawCircle(220, row+20, 28, 0xFFFF);

        if (digitalRead(led_test_points[i])) {
            tft.fillCircle(220, row+20, 25, 0xFFFF);            // draw a bright LED
            // test_pass &= false;
            led_pass[i] &= false;
        }
        else {
            tft.fillCircle(220, row+20, 25, 0x39C7);            // draw a dimmed LED
            
        }
        delay(250);
        Serial.write(0x3C+i);
        uint32_t t_s = millis();
        for (;;) {
            if (Serial.available()) {
                uint8_t u = Serial.read();
                if (u == 0x03) {                                // ACK from DUT
                    if (digitalRead(led_test_points[i])) {      // DUT is supposed to pull PLnn HIGH
                        tft.fillCircle(220, row+20, 25, 0xFFFF);
                        break;
                    }
                    else {
                        tft.fillCircle(220, row+20, 25, 0x39C7);
                        // test_pass &= false;
                        led_pass[i] &= false;
                        break;
                    }
                }
            }
            if (millis()-t_s>=2000) {
                comm_lost(tft);
            }
        }
        delay(250);

        if (led_pass[i]) {
            tft.fillCircle(220, row+20, 25, color_pass);
        }
        else {
            tft.fillCircle(220, row+20, 25, color_fail);
        }
        
        row += 90;
    }

    /* any failure should bring test_pass to 'false' */
    test_pass = led_pass[0] && led_pass[1] && led_pass[2] && led_pass[3];
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