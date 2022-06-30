#ifndef SD_H
#define SD_H

void sd_init();
void sd_send_command(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t sd_read();
uint8_t sd_go_idle();
void sd_read_res7(uint8_t* res);
void sd_send_if_cond(uint8_t* res);
void sd_print_response(uint8_t* res);

#endif