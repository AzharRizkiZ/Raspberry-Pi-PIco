#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lcd1602_i2c.h"

// Konstanta konversi ADC → suhu
const float conversion_factor = 3.3f / (1 << 12);

int main() {
    stdio_init_all();

    // Init I2C di GP0 (SCL) dan GP1 (SDA)
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    // Init LCD
    lcd_init();

    // Init ADC internal
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while (true) {
        uint16_t raw = adc_read();
        float voltage = raw * conversion_factor;
        float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;

        char buffer[17];
        snprintf(buffer, sizeof(buffer), "Temp: %.2f C", temperature);

        lcd_clear();
        lcd_set_cursor(0, 0);
        lcd_print("Realtime Temp");
        lcd_set_cursor(0, 1);
        lcd_print(buffer);

        sleep_ms(1000);
    }
}
