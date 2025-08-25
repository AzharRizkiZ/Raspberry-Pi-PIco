#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lcd1602_i2c.h"

#define SERVO_PIN 18
#define LED_PIN 16
#define BUTTON_PIN 15   // switch ON/OFF
#define PIR_PIN 2

uint slice_num;

// === Servo function (0° – 180°) ===
void servo_set_angle(uint angle) {
    // Servo SG90: 0.5ms (0°) – 2.5ms (180°) pada periode 20ms (50Hz)
    float min_pulse = 0.5f, max_pulse = 2.5f;
    float duty_ms = min_pulse + (angle / 180.0f) * (max_pulse - min_pulse);
    uint16_t wrap = 49999; // harus sama dengan pwm_set_wrap
    uint16_t level = (uint16_t)((duty_ms / 20.0f) * (wrap + 1));
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SERVO_PIN), level);
}

int main() {
    stdio_init_all();

    // --- LED ---
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    // --- Switch (ON/OFF) ---
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);  // gunakan pull-up internal

    // --- Servo (PWM) ---
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_set_wrap(slice_num, 49999);   // periode 20ms (50Hz)
    pwm_set_clkdiv(slice_num, 50.0f); // clock divider
    pwm_set_enabled(slice_num, true);
    servo_set_angle(0); // awal posisi 0°

    // --- PIR ---
    gpio_init(PIR_PIN);
    gpio_set_dir(PIR_PIN, GPIO_IN);

    // --- ADC internal temperature ---
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    // --- I2C LCD ---
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);
    lcd_init();
    lcd_clear();

    char line1[17], line2[17];
    while (1) {
        // PIR check
        bool pir_motion = gpio_get(PIR_PIN);

        // Switch state (0 = ON, 1 = OFF karena pull-up)
        bool switch_on = (gpio_get(BUTTON_PIN) == 0);

        if (switch_on) {
            servo_set_angle(90);     // servo ke 90°
            gpio_put(LED_PIN, 1);    // LED nyala
        } else {
            servo_set_angle(0);      // servo ke 0°
            gpio_put(LED_PIN, 0);    // LED mati
        }

        // Temperature
        uint16_t raw = adc_read();
        const float conversion_factor = 3.3f / (1 << 12);
        float volts = raw * conversion_factor;
        float tempC = 27 - (volts - 0.706f) / 0.001721f;

        // LCD display
        snprintf(line1, 17, "Temp: %.1fC", tempC);
        snprintf(line2, 17, "SW:%s PIR:%s",
                 switch_on ? "ON " : "OFF",
                 pir_motion ? "YES" : "NO ");

        lcd_set_cursor(0,0); lcd_print(line1);
        lcd_set_cursor(0,1); lcd_print(line2);

        sleep_ms(200);
    }
}
