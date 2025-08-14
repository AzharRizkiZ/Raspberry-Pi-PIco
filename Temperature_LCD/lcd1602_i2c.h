#ifndef LCD1602_I2C_H
#define LCD1602_I2C_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>

#define LCD_ADDR 0x27  // Alamat I2C modul PCF8574, bisa 0x27 atau 0x3F
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE_BIT 0x04

void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_init();
void lcd_clear();
void lcd_set_cursor(int col, int row);
void lcd_print(const char *str);

#endif
