#ifndef UI_H
#define UI_H

void touch(Adafruit_TFTLCD &tft, uint16_t x_0, uint16_t x_1, uint16_t y_0, uint16_t y_1);
int touch(Adafruit_TFTLCD &tft, uint16_t x_0, uint16_t x_1, uint16_t y_0, uint16_t y_1, uint16_t x_2, uint16_t x_3, uint16_t y_2, uint16_t y_3);
int touch(Adafruit_TFTLCD &tft, uint16_t x_0, uint16_t x_1, uint16_t y_0, uint16_t y_1, uint16_t x_2, uint16_t x_3, uint16_t y_2, uint16_t y_3, uint32_t timeout);
int touch9(Adafruit_TFTLCD &tft);
void comm_lost(Adafruit_TFTLCD &tft);

#endif