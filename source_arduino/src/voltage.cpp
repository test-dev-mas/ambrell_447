#include "essentials.hpp"
#include "voltage.hpp"
#include "ui.hpp"

#define CS 20                               //Selection Pin
#define DOUT 51                              //MOSI // SDI of Multimeter click is connected to here
#define DIN  50                              //MISO // SDO of Multimeter click is connected to here
#define SCK    PB1                            //Clock

char* test_points[] = {"TP5 (5V)",
                        "TP14 (3.3V)",
                        "J13.3",
                        "J13.6",
                        "J13.3 TERMINATED",
                        "J13.6 TERMINATED",
                        "J13.5",
                        "J13.4",
                        "J13.7",
                        "J13.8",
                        "J2.4",
                        "J2.5",
                        "J2.7 (E_stop Open)",
                        "J2.7 (E_stop Close)",
                        "J2.8 (GND)",
                        "J16.2 (Low)",
                        "J16.2 (High)"}; 

uint8_t relay[] = {RT1, RT2, RT3, RT6, RT3, RT6, RT5, RT4, RT7, RT8, RT17, RT18, RT19, RT19, RT20, RT21, RT21};
float limits[][2] = {{4.75, 5.25}, {3.0, 3.6}, {4.75, 5.25}, {-0.165, 0.165}, {3.6, 4.4}, {0.9, 1.2}, {0.7, 1.1}, {-0.165, 0.165}, {0.7, 1.1}, {-0.165, 0.165}, {23, 25}, {23, 25}, {-0.165, 0.165}, {11.4, 12.6}, {-0.165, 0.165}, {-0.165, 0.165}, {11.4, 12.6}};
static bool test_pass = true;

bool measure(Adafruit_TFTLCD &tft) {
    tft.fillScreen(0x2104);

    tft.fillRect(0, 0, 320, 60, color_primary);
    tft.setCursor(30, 30);
    tft.println("Voltage Test");

    /* draw first column */
    uint16_t row = 60;
    for (auto i=0;i<sizeof(test_points)/sizeof(char*);i++) {
        row += 20;
        tft.setCursor(0, row);
        tft.println(test_points[i]);
    }
    /* draw second column */
    row = 60;
    for (auto i=0;i<sizeof(test_points)/sizeof(char*);i++) {
        int adc_value_i;
        float adc_value_f;
        /* edge cases (turn on/off MOSFET) */
        if (i==4) {
            Serial.write(0x47);                 // TERM_RS485 ON
            uint32_t t_s = millis();            // time-stampe after sending command
            for (;;) {                          // pause flow of program until receiving ACK from DUT
                if (Serial.available()) {
                    uint8_t u = Serial.read();
                    if (u == 0x03) {            // ACK from DUT
                    break;
                    }
                }
                if (millis()-t_s>=2000) {       // a timeout is necessary
                    break;
                }
            }
        }
        else if (i==6) {                        // this should be done after "J13.6 TERMINATED" is sampled
            Serial.write(0x48);                 // TERM_RS485 OFF
            uint32_t t_s = millis();            // time-stampe after sending command
            for (;;) {                          // pause flow of program until receiving ACK from DUT
                if (Serial.available()) {
                    uint8_t u = Serial.read();
                    if (u == 0x03) {            // ACK from DUT
                    break;
                    }
                }
                if (millis()-t_s>=2000) {       // a timeout is necessary
                    break;
                }
            }
        }
        if (i==12 || i==15) {
            Serial.write(0x40);
            uint32_t t_s = millis();            // time-stampe after sending command
            for (;;) {                          // pause flow of program until receiving ACK from DUT
                if (Serial.available()) {
                    uint8_t u = Serial.read();
                    if (u == 0x03) {            // ACK from DUT
                    break;
                    }
                }
                if (millis()-t_s>=2000) {       // a timeout is necessary (a timeout of 1000 is implementd on DUT, this one should be longer (i.e. 2000))
                    break;
                }
            }
        }
        else if (i==13 || i==16) {
            Serial.write(0x41);
            uint32_t t_s = millis();            // time-stampe after sending command
            for (;;) {                          // pause flow of program until receiving ACK from DUT
                if (Serial.available()) {
                    uint8_t u = Serial.read();
                    if (u == 0x03) {            // ACK from DUT
                    break;
                    }
                }
                if (millis()-t_s>=2000) {       // a timeout is necessary
                    break;
                }
            }
        }
        /* select relay */
        setup_relay(relay[i]);
        /* read ADC */
        adc_value_i = read_adc();
        /* edge cases (voltage divider) */
        if (i==10 || i==11) {
            adc_value_f = adc_value_i/12.1;
        }
        else {
            adc_value_f = adc_value_i/121.0;
        }
        /* limit check */
        if (adc_value_f >= limits[i][0] && adc_value_f <= limits[i][1]) {
            tft.setTextColor(0x66c2);
        }
        else {
            test_pass &= false; 
            tft.setTextColor(0xF8A8);
        }
        row += 20;
        tft.setCursor(230, row);
        char fs[9] = {0};
        dtostrf(adc_value_f, 5, 3, fs);
        strcat(fs, "V");
        tft.println(fs);
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

int read_adc(){
    int adcvalue3 = -2045;
    byte commandbits3 = B11000000; //command bits - start, mode, chn (3), dont care (3)
    //allow channel selection
    int channel3 = 1;
    commandbits3|=((channel3-0)<<3);
    digitalWrite(CS,LOW); //Select adc
    // setup bits to be written
    for (int i=7; i>=3; i--){
    digitalWrite(DOUT,commandbits3&1<<i);
    //cycle clock
    PORTB |= (1<<SCK);
    PORTB &= ~(1<<SCK);    
    }
    PORTB |= (1<<SCK);    //ignores 2 null bits
    PORTB &= ~(1<<SCK);
    PORTB |= (1<<SCK);  
    PORTB &= ~(1<<SCK);
    //read bits from adc
    for (int i=11; i>=0; i--){
    adcvalue3+=digitalRead(DIN)<<i;
    //cycle clock
    PORTB |= (1<<SCK);
    PORTB &= ~(1<<SCK);
    }
    digitalWrite(CS, HIGH); //turn off device
    // Serial.println(adcvalue3, BIN);
    return adcvalue3;
}

void init_adc() {
    for (auto i=2;i<10;i++) {                   // set up relays 1~7 & 17~24
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
    
    pinMode(12, OUTPUT);                        // set up relay 68 (CONNECT V+ ON METER TO TEST POINT(PIN 4 OF RELAY 1))
    pinMode(13, OUTPUT);
    digitalWrite(12, HIGH);                     // TURN ON RELAY 68
    digitalWrite(13, LOW);

    pinMode(CS, OUTPUT);                    // set up multimeter click
    pinMode(DOUT, OUTPUT);
    pinMode(DIN, INPUT);
    DDRB |= (1<<SCK);
    digitalWrite(CS,HIGH);
    digitalWrite(DOUT,LOW);
    PORTB &= ~(1<<SCK);
}

void setup_relay(uint8_t pins) {
    digitalWrite(2, (pins>>7));                 // U1  
    digitalWrite(3, (pins&0x40)>>6);            // U2
    digitalWrite(4, (pins&0x20)>>5);            // U3
    digitalWrite(5, (pins&0x10)>>4);            // U4
    digitalWrite(6, (pins&0x08)>>3);            // A0 on Un
    digitalWrite(7, (pins&0x04)>>2);            // A1 on Un
    digitalWrite(8, (pins&0x02)>>1);            // A2 on Un
    digitalWrite(9, (pins&0x01));               // A3 on Un
    delay(500);                                 // allow some time for voltage to stablize
}