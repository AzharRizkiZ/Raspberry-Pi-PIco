#include "lcd1602_i2c.h"

static void lcd_write(uint8_t data) {
    i2c_write_blocking(i2c0, LCD_ADDR, &data, 1, false);
}

static void lcd_toggle_enable(uint8_t val) {
    sleep_us(600);
    lcd_write(val | LCD_ENABLE_BIT);
    sleep_us(600);
    lcd_write(val & ~LCD_ENABLE_BIT);
    sleep_us(600);
}

void lcd_send_cmd(uint8_t cmd) {
    uint8_t high = (cmd & 0xF0) | LCD_BACKLIGHT;
    uint8_t low  = ((cmd << 4) & 0xF0) | LCD_BACKLIGHT;
    lcd_write(high);
    lcd_toggle_enable(high);
    lcd_write(low);
    lcd_toggle_enable(low);
}

void lcd_send_data(uint8_t data) {
    uint8_t high = (data & 0xF0) | LCD_BACKLIGHT | 0x01;
    uint8_t low  = ((data << 4) & 0xF0) | LCD_BACKLIGHT | 0x01;
    lcd_write(high);
    lcd_toggle_enable(high);
    lcd_write(low);
    lcd_toggle_enable(low);
}

void lcd_init() {
    sleep_ms(50);
    lcd_send_cmd(0x33);
    lcd_send_cmd(0x32);
    lcd_send_cmd(0x28); // 4-bit mode, 2 line, 5x8 font
    lcd_send_cmd(0x0C); // Display on, cursor off
    lcd_send_cmd(0x06); // Increment cursor
    lcd_clear();
}

void lcd_clear() {
    lcd_send_cmd(0x01);
    sleep_ms(2);
}

void lcd_set_cursor(int col, int row) {
    int row_offsets[] = {0x00, 0x40};
    lcd_send_cmd(0x80 | (col + row_offsets[row]));
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}
