/* blink on both J11.1 & J10.1 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define BLINK_MS    100
#define J11_1       PH4     // J11 PIN 1
#define J10_1       PL0     // READY LED

int main() {
    //
    DDRH = (1<<PH4);
    DDRL = (1<<J10_1);

    while (true) {
        PORTH = (1<<J11_1);
        PORTL = (1<<J10_1);
        _delay_ms(BLINK_MS);
        PORTH &= ~(1<<J11_1);
        PORTL &= ~(1<<J10_1);
        _delay_ms(BLINK_MS);
    }
}