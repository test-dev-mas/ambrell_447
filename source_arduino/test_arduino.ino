#include <Adafruit_GFX.h>       // Core graphics library
#include <Adafruit_TFTLCD.h>    // Hardware-specific library
#include <avr/wdt.h>

#include "src/essentials.hpp"
#include "src/voltage.hpp"
#include "src/start.hpp"
#include "src/test_2a.hpp"
#include "src/test_2b.hpp"
#include "src/test_2c.hpp"
#include "src/test_3a.hpp"
#include "src/test_3b.hpp"
#include "src/test_3c.hpp"
#include "src/test_3d.hpp"
#include "src/ui.hpp"
#include "src/end.hpp"
#include "src/debug_mode.hpp"

#define LCD_CS 21               // Chip Select
#define LCD_CD 19               // Command/Data
#define LCD_WR 18               // LCD Write
#define LCD_RD 17               // LCD Read
#define LCD_RESET 16            // Can alternately just connect to Arduino's reset pin

// void reset_controller();
// void reset_dut();

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup() {
    init_adc();                                 // initialize adc, this also disables all relays
    Serial.begin(115200);

    tft.reset();
    uint16_t identifier = tft.readID();
    tft.begin(identifier);

    if (start(tft)) {
        for (;;) {
            turn_on_dut();
            debug_mode(tft, "Debug Mode");      // debug_mode shall not exit unless resets
            reset_controller();
            turn_off_dut();
        }
    }

    turn_on_dut();

    uint32_t t_s = millis();
    for (;;) {
        if (Serial.available()) {
            uint8_t u = Serial.read();
            if (u == 0x02) {                    // waiting for "system ready" flag
                break;
            }
        }
        if (millis()-t_s>=7000) {
            comm_lost(tft);
        }
    }

    /* voltage measurement test */
    if (!(measure(tft))) {                      // if test fails
        reset_controller();
    }
    
    if (!(test_2a(tft, "Contact Open & Close"))) {
        reset_controller();
    }

    if (!(test_2b(tft, "Key Press Test"))) {
        reset_controller();
    }

    if (!(test_2c(tft, "LED Test"))) {
        reset_controller();
    }

    if (!(test_3a(tft, "RS485 Test"))) {
        reset_controller();
    }

    if (!(test_3b(tft, "USB Test"))) {
        reset_controller();
    }

    if (!(test_3c(tft, "SD Test"))) {
        reset_controller();
    }

    if (!(test_3d(tft, "Display Test"))) {
        reset_controller();
    }

    turn_off_dut();
    end(tft);
    reset_controller();
}

void loop() {
}

void reset_controller() {
    wdt_enable(WDTO_15MS);
    while (1) {
        //
    }
}

void reset_dut() {
    DDRC |= (1<<PC0);                           // D37 ON TB6.1, this line resets DUT
    delay(1);                                   // minimum pulse width is 2.5 micro second for atmega2560
    PORTC |= (1<<PC0);
    DDRC &= ~(1<<PC0);                          // relinquish control for DUT USB AVRISP
}

void turn_on_dut() {
    /* TURN ON DUT BY RELAY */
    pinMode(36, OUTPUT);
    digitalWrite(36, HIGH);
    RT67_ON
    delay(50);                                  // wait for voltage to stablize
}

void turn_off_dut() {
    /* TURN ON DUT BY RELAY */
    pinMode(36, OUTPUT);
    digitalWrite(36, LOW);
    RT67_OFF
}