#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd1602_i2c.h"

#define PIR_PIN 2     // PIR sensor di GP2
#define LED_PIN 25    // LED internal Pico di GP25

int main() {
    stdio_init_all();

    // Init I2C0 di GP0 (SDA) & GP1 (SCL)
    i2c_init(i2c0, 100 * 1000);   // 100kHz
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    // Init PIR (tanpa pull)
    gpio_init(PIR_PIN);
    gpio_set_dir(PIR_PIN, GPIO_IN);
    // gpio_pull_down(PIR_PIN);

    // Init LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Init LCD
    lcd_init();
    lcd_clear();
    lcd_set_cursor(0,0);
    lcd_print("PIR + LCD Demo");

    // --- Calibrating PIR ---
    lcd_set_cursor(0,1);
    lcd_print("Calibrating...");
    sleep_ms(30000);   // tunggu ±30 detik agar PIR stabil

    lcd_clear();
    lcd_set_cursor(0,0);
    lcd_print("System Ready");
    sleep_ms(2000);
    lcd_clear();

    while (true) {
        int pir_state = gpio_get(PIR_PIN);

        if (pir_state) {
            gpio_put(LED_PIN, 1);
            lcd_clear();
            lcd_set_cursor(0,0);
            lcd_print("Motion: Detected");
            lcd_set_cursor(0,1);
            lcd_print("LED: ON");
        } else {
            gpio_put(LED_PIN, 0);
            lcd_clear();
            lcd_set_cursor(0,0);
            lcd_print("Motion: NONE");
            lcd_set_cursor(0,1);
            lcd_print("LED: OFF");
        }

        sleep_ms(500);
    }
}
