#include <avr/io.h>
#include <util/delay.h>

#include "hx8357.h"

#define CS  PC2
#define CD  PC3
#define WR  PC0
#define RD  PC1
#define RST PC4

#define DELAY7                                                                 \
  asm volatile("rjmp .+0"                                                      \
               "\n\t"                                                          \
               "rjmp .+0"                                                      \
               "\n\t"                                                          \
               "rjmp .+0"                                                      \
               "\n\t"                                                          \
               "nop"                                                           \
               "\n" ::);

void write8_command(uint8_t command) {
    PORTC &= ~(1<<CS);
    PORTC &= ~(1<<CD);                      // command
    PORTA = command;                        // PORTA maps to D7...0
    
    PORTC &= ~(1<<WR);
    PORTC |= (1<<WR);                       // strobe WR

    PORTC |= (1<<CS);
}

void write8_data(uint8_t data) {
    PORTC &= ~(1<<CS);
    PORTC |= (1<<CD);                       // data
    PORTA = data;                           // PORTA maps to D7...0
    
    PORTC &= ~(1<<WR);
    PORTC |= (1<<WR);                       // strobe WR
    
    PORTC |= (1<<CS);
}

uint16_t read_pixel() {
    uint8_t hi;
    uint8_t lo;

    PORTC &= ~(1<<CS);
    PORTC &= ~(1<<CD);
    PORTA = 0x2E;                           // read GRAM command

    PORTC &= ~(1<<WR);
    PORTC |= (1<<WR);

    PORTA = 0xFF;
    DDRA = 0x00;

    PORTC |= (1<<CD);                       // drive HIGH to read
    
    PORTC &= ~(1<<RD);                      // first two reads return dummy data
    DELAY7;
    hi = PINA;
    PORTC |= (1<<RD);

    PORTC &= ~(1<<RD);                      // first two reads return dummy data
    DELAY7;
    hi = PINA;
    PORTC |= (1<<RD);

    PORTC &= ~(1<<RD);                      // 3rd read returns valid data
    DELAY7;
    hi = PINA;
    PORTC |= (1<<RD);

    PORTC &= ~(1<<RD);                      // 4th reads returns valid data
    DELAY7;
    lo = PINA;
    PORTC |= (1<<RD);
    
    DDRA = 0xFF;
    PORTC |= (1<<CS);
    
    return ((uint16_t)hi << 8) | lo;
}

void hx8357_init() {
    DDRC |= (1<<CS) | (1<<CD) | (1<<WR) | (1<<RD) | (1<<RST);
    PORTC |= (1<<CS) | (1<<CD) | (1<<WR) | (1<<RD) | (1<<RST);

    DDRA = 0xFF;

    write8_command(0x01);                   // software reset
    _delay_ms(10);

    write8_command(0xB9);                   // enable extension command
    write8_data(0xFF);
    write8_data(0x83);
    write8_data(0x57);
    _delay_ms(250);

    write8_command(0xB3);                   // set RGB interface
    write8_data(0x00);
    write8_data(0x00);
    write8_data(0x06);
    write8_data(0x06);

    write8_command(0xB6);                   // set VCOM voltage
    write8_data(0x25);

    write8_command(0xB0);                   // set internal oscillator
    write8_data(0x68);

    write8_command(0xCC);                   // set panel characteristic register
    write8_data(0x05);

    write8_command(0xB1);                   // set power control
    write8_data(0x00);
    write8_data(0x15);
    write8_data(0x1C);
    write8_data(0x1C);
    write8_data(0x83);
    write8_data(0xAA);

    write8_command(0xC0);                   // set source circuit option
    write8_data(0x50);
    write8_data(0x50);
    write8_data(0x01);
    write8_data(0x3C);
    write8_data(0x1E);
    write8_data(0x08);

    write8_command(0xB4);                   //set display cycle register (OPTIONAL)
    write8_data(0x02);
    write8_data(0x40);
    write8_data(0x00);
    write8_data(0x2A);
    write8_data(0x2A);
    write8_data(0x0D);
    write8_data(0x78);

    // write8_command(0xE0);                   // set gamma
    // write8_data(0x02);
    // write8_data(0x0A);
    // write8_data(0x11);
    // write8_data(0x1D);
    // write8_data(0x23);
    // write8_data(0x35);
    // write8_data(0x41);
    // write8_data(0x4b);
    // write8_data(0x4b);
    // write8_data(0x42);
    // write8_data(0x3A);
    // write8_data(0x27);
    // write8_data(0x1B);
    // write8_data(0x08);
    // write8_data(0x09);
    // write8_data(0x03);
    // write8_data(0x02);
    // write8_data(0x0A);
    // write8_data(0x11);
    // write8_data(0x1d);
    // write8_data(0x23);
    // write8_data(0x35);
    // write8_data(0x41);
    // write8_data(0x4b);
    // write8_data(0x4b);
    // write8_data(0x42);
    // write8_data(0x3A);
    // write8_data(0x27);
    // write8_data(0x1B);
    // write8_data(0x08);
    // write8_data(0x09);
    // write8_data(0x03);
    // write8_data(0x00);
    // write8_data(0x01);

    write8_command(0x3A);                   // pixel interface format (COLMOD)
    write8_data(0x06);

    write8_command(0x36);                   // memory access control (MADCTL)
    write8_data(0xC0);

    write8_command(0x35);                   // tear on
    write8_data(0x00);

    write8_command(0x44);                   // set tear scan line
    write8_data(0x00);
    write8_data(0x02);

    write8_command(0x11);                   // sleep out
    _delay_ms(150);

    write8_command(0x29);                   // display on
    _delay_ms(50);

    // write8_command(0x2A);                   // set column address (CODE WORKED AFTER TAKING OUT THIS PART!)
    // write8_data(0x00);                      // start higher byte
    // write8_data(0x00);                      // start lower byte
    // write8_data(0x01);                      // end higher byte
    // write8_data(0x3F);                      // end LOWER byte

    // write8_command(0x2B);                   // set page address (CODE WORKED AFTER TAKING OUT THIS PART!)
    // write8_data(0x00);
    // write8_data(0x00);
    // write8_data(0x01);
    // write8_data(0xDF);

    /* clear screen */
    // fill_screen(0xff, 0x00, 0x00);
    // fill_screen(0x00,0xff,0x00);
    fill_screen(0x00,0x00,0xff);
}

void fill_screen(uint8_t r, uint8_t g, uint8_t b) {
    write8_command(0x2C);                   // write to GRAM

    for (uint16_t i=0;i<480;i++) {
        for (uint16_t j=0;j<320;j++) {
            write8_data(r);              //
            write8_data(g);              //
            write8_data(b);              //
        }
    }
}

void draw_gradient() {
    write8_command(0x2C);                           // write to GRAM

    for (uint16_t i=0;i<480;i++) {                  // checker pattern
        for (uint16_t j=0;j<320;j++) {
            float x = j/320.0 + 1/640.0;
            float y = i/480.0 + 1/960.0;

            x *= 5;
            y *= 7;
            
            uint8_t m = floorf(x);
            uint8_t n = floorf(y);

            if ((m%2) == (n%2)) {
                write8_data(0x00);
                write8_data(0x00);
                write8_data(0x00);
            }
            else {
                write8_data(0xff);
                write8_data(0xff);
                write8_data(0xFF);
            }
        }
    }

    // for (uint16_t i=0;i<480;i++) {                  // color gradient
    //     float y = i/480.0 + 1/960.0;
    //     if (y < 1/7.0) {
    //         for (uint16_t j=0;j<320;j++) {
    //             write8_data(0xBD);
    //             write8_data(0xB2);
    //             write8_data(0xFF);
    //         }
    //     }
    //     if (y > 1/7.0 && y < 2/7.0) {
    //         for (uint16_t j=0;j<320;j++) {
    //             write8_data(0xA0);
    //             write8_data(0xC4);
    //             write8_data(0xFF);
    //         }
    //     }
    //     if (y > 2/7.0 && y < 3/7.0) {
    //         for (uint16_t j=0;j<320;j++) {
    //             write8_data(0x9B);
    //             write8_data(0xF6);
    //             write8_data(0xFF);
    //         }
    //     }
    //     if (y > 3/7.0 && y < 4/7.0) {
    //         for (uint16_t j=0;j<320;j++) {
    //             write8_data(0xCA);
    //             write8_data(0xFF);
    //             write8_data(0xBF);
    //         }
    //     }
    //     if (y > 4/7.0 && y < 5/7.0) {
    //         for (uint16_t j=0;j<320;j++) {
    //             write8_data(0xfD);
    //             write8_data(0xFF);
    //             write8_data(0xB6);
    //         }
    //     }
    //     if (y > 5/7.0 && y < 6/7.0) {
    //         for (uint16_t j=0;j<320;j++) {
    //             write8_data(0xFF);
    //             write8_data(0xD6);
    //             write8_data(0xA5);
    //         }
    //     }
    //     if (y > 6/7.0) {
    //         for (uint16_t j=0;j<320;j++) {
    //             write8_data(0xff);
    //             write8_data(0xAD);
    //             write8_data(0xAD);
    //         }
    //     }
    // }
}