#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "sd.h"
#include "spi.h"
#include "uart.h"

#define CMD0        0
#define CMD0_ARG    0x00000000
#define CMD0_CRC    0x94
#define CMD8        8
#define CMD8_ARG    0x000001AA
#define CMD8_CRC    0x86
#define CS          PB0

void sd_init() {
    _delay_ms(1);

    for (uint8_t i=0;i<10;i++) {
        spi_transfer(0xFF);
    }
}

void sd_send_command(uint8_t cmd, uint32_t arg, uint8_t crc) {
    spi_transfer(cmd|0x40);                 // most significant two bits should be 01

    spi_transfer((uint8_t)(arg>>24));
    spi_transfer((uint8_t)(arg>>16));
    spi_transfer((uint8_t)(arg>>8));
    spi_transfer((uint8_t)arg);

    spi_transfer(crc|0x01);                 // crc is 7-bit long, last bit '1' is the end bit
}

uint8_t sd_read() {
    uint8_t i = 0;
    uint8_t res;
    /* CS is high initially, res reads 0xFF, but SD should respond within 8 clock cycles */
    while ((res=spi_transfer(0xFF)) == 0xFF) {
        i++;
        if (i>8) {
            break;
        }
    }
    return res;
}

uint8_t sd_go_idle() {
    spi_transfer(0xFF);
    PORTB &= ~(1<<CS);                      // enable CS
    spi_transfer(0xFF);

    sd_send_command(CMD0, CMD0_ARG, CMD0_CRC);

    uint8_t res = sd_read();

    spi_transfer(0xFF);
    PORTB |= (1<<CS);                       // disable CS
    spi_transfer(0xFF);

    return res;
}

void sd_read_res7(uint8_t* res) {
    res[0] = sd_read();

    if (res[0] > 1) {
        return;
    }

    res[1] = spi_transfer(0xFF);
    res[2] = spi_transfer(0xFF);
    res[3] = spi_transfer(0xFF);
    res[4] = spi_transfer(0xFF);
}

void sd_send_if_cond(uint8_t* res) {
    spi_transfer(0xFF);
    PORTB &= ~(1<<CS);                      // enable CS
    spi_transfer(0xFF);

    sd_send_command(CMD8, CMD8_ARG, CMD8_CRC);

    sd_read_res7(res);

    spi_transfer(0xFF);
    PORTB |= (1<<CS);                       // disable CS
    spi_transfer(0xFF);
}

void sd_print_response(uint8_t* res) {
    //
    char message_res[10] = {0};
    
    uart2_puts("Response:\r\n\t");

    sprintf(message_res, "%02X", res[0]);     // convert int to string
    uart2_puts("Idle State: ");
    uart2_puts(message_res);
    uart2_puts("\r\n");                     // carriage return & newline for serial terminal

    sprintf(message_res, "%02X", res[4]);
    uart2_puts("Echo: ");
    uart2_puts(message_res);
    uart2_puts("\r\n");
}