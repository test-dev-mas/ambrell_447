#ifndef HX8357_H
#define HX8357_H

void write8_command(uint8_t command);
void write8_data(uint8_t data);
uint16_t read_pixel();
void hx8357_init();
void hx8357_test();
void fill_screen(uint8_t r, uint8_t g, uint8_t b);
void blink(uint8_t count);
void draw_gradient();

#endif