#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "hx8357.h"
#include "sd.h"
#include "spi.h"
#include "uart.h"

volatile bool uart_flag = false;
volatile uint8_t temp;

int main() {
    hx8357_init();
    spi_init();
    uart0_init();
    uart1_init();
    uart2_init();
    uart3_init();
    sd_init();
    
    // uart2_puts("hello world");

    DDRL |= (1<<PL0)|(1<<PL1)|(1<<PL2)|(1<<PL3);        // THIS LINE PUTS LEDS(LIMIT,FAULT,...) TO LOW, IF NOT PIN READ GIVES RANDOM RESULTS
    
    sei();                                      // enable golabl interrupt  

    uart3_transmit(0x02);                       // system ready flag

    for (;;) {
        if (uart_flag) {
            uart_flag = false;
            if (temp == 0x30) {
                DDRH |= (1<<PH4);
                PORTH &= ~(1<<PH4);
                uart3_transmit(0x03);
            }
            if (temp == 0x31) {
                DDRH |= (1<<PH5);
                PORTH &= ~(1<<PH5);
                uart3_transmit(0x03);
            }
            if (temp == 0x32) {
                if (PINL&(1<<PL6)) {            // if PL6(KP8) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x33) {
                if (PINL&(1<<PL7)) {            // if PL7(KP7) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x34) {
                if (PIND&(1<<PD4)) {            // if PL6(KP8) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x35) {
                if (PIND&(1<<PD5)) {            // if PL6(KP8) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x36) {
                if (PIND&(1<<PD6)) {            // if PL6(KP8) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x37) {
                if (PIND&(1<<PD7)) {            // if PL6(KP8) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x38) {
                if (PING&(1<<PG0)) {            // if PL6(KP8) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x39) {
                if (PIND&(1<<PD0)) {            // if PL6(KP8) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x3A) {
                if (PIND&(1<<PD1)) {            // if PL6(KP8) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x3B) {
                if (PING&(1<<PG1)) {            // if PG1(RAR) is HIGH, meaning not pressed
                    uart3_transmit(0x2B);       // send "ON" signal
                }
                else {
                    uart3_transmit(0x2C);       // send "OFF" signal
                }
            }
            if (temp == 0x3C) {
                // DDRL |= (1<<PL3);
                PORTL |= (1<<PL3);              // turn on LED
                uart3_transmit(0x03);           // send ACK
            }
            if (temp == 0x3D) {
                // DDRL |= (1<<PL2);
                PORTL |= (1<<PL2);
                uart3_transmit(0x03);           // send ACK
            }
            if (temp == 0x3E) {
                // DDRL |= (1<<PL1);
                PORTL |= (1<<PL1);
                uart3_transmit(0x03);           // send ACK
            }
            if (temp == 0x3F) {
                // DDRL |= (1<<PL0);
                PORTL |= (1<<PL0);
                uart3_transmit(0x03);           // send ACK
            }
            if (temp == 0x40) {
                DDRE |= (1<<PE3);
                PORTE |= (1<<PE3);
                uart3_transmit(0x03);           // send ACK
            }
            if (temp == 0x41) {
                DDRE |= (1<<PE3);               // enable Q5 at J16
                PORTE &= ~(1<<PE3);
                uart3_transmit(0x03);           // send ACK
            }
            if (temp == 0x42) {                 // RS485 test
                DDRG |= (1<<PG2);               // enable receive on SN65HVD3085E
                PORTG &= ~(1<<PG2);

                uint16_t count = 1000;          // timeout ~1000 ms

                uart0_transmit(0xAA);
                while(!(UCSR0A&(1<<RXC0)) && (count>0)) {
                    count--;                    // a simple timeout routine
                    _delay_ms(1);
                };
                if (UDR0 == 0xAA) {
                    uart3_transmit(0x2B);
                }
                else {
                    uart3_transmit(0x2C);
                }
            }
            if (temp == 0x43) {                 // USB test
                uint16_t count = 1000;          // timeout ~1000 ms

                uart2_transmit(0xAA);
                // _delay_ms(1000);
                while(!(UCSR2A&(1<<RXC2)) && (count>0)) {
                    count--;
                    _delay_ms(1);
                }
                if (UDR2 == 0xAA) {
                    uart3_transmit(0x2B);
                }
                else {
                    uart3_transmit(0x2C);
                }
            }
            /* SD test */
            if (temp == 0x44) {
                uint8_t res[5] = {0};

                sd_go_idle();                   // this step is necessary
                sd_send_if_cond(res);           // writes response to res[]
                // sd_print_response(res);         // this line not necessary, sprintf() sometimes resets DUT
                if (res[4] == 0xAA) {
                    uart3_transmit(0x2B);
                }
                else {
                    uart3_transmit(0x2C);
                }
            }
            /* display test
            
                both +/- channels have to be broken for transmit to fail
                maybe tx/rx is done on a same chip
                different from rs485 setup!
             */
            if (temp == 0x45) {
                DDRH |= (1<<PH7);
                PORTH |= (1<<PH7);              // enable transmit on SN65HVD1793

                uint16_t count = 1000;          // timeout ~1000 ms

                uart1_transmit(0xAA);
                while(!(UCSR1A&(1<<RXC1)) && (count>0)) {
                    count--;
                    _delay_ms(1);
                }
                if (UDR1 == 0xAA) {
                    uart3_transmit(0x2B);
                }
                else {
                    uart3_transmit(0x2C);
                }
                // wait for controller to acknowlage (NOT WORKING!! ITS INSIDE THIS IF CONDITION)
                // while(!(UCSR1A&(1<<RXC1)) && (count>0)) {
                //     count--;
                //     _delay_ms(1);
                // }
                // if (UDR1 == 0x03) {
                //     draw_gradient();
                // }
            }
            if (temp == 0x46) {
                draw_gradient();
            }
            /* TERM_RS485 ON */
            if (temp == 0x47) {
                DDRE |= (1<<PE2);
                PORTE |= (1<<PE2);
                uart3_transmit(0x03);           // send ACK
            }
            /* TERM_RS485 OFF */
            if (temp == 0x48) {
                DDRE |= (1<<PE2);
                PORTE &= ~(1<<PE2);
                uart3_transmit(0x03);           // send ACK
            }
        }
    }
}

ISR(USART3_RX_vect) {
    temp = UDR3;
    uart_flag = true;
}