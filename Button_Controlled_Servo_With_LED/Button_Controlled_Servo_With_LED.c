#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

#define SERVO_PIN 18   // Servo signal (PWM)
#define LED_PIN   16   // Status LED
#define SW_PIN    15   // Switch input

// Konversi derajat ke microsecond (0.5ms - 2.5ms)
uint16_t angle_to_pwm_us(int angle) {
    return 500 + (angle * 2000) / 180;
}

// Atur posisi servo
void set_servo_angle(uint slice_num, uint channel, int angle) {
    uint16_t us = angle_to_pwm_us(angle);
    uint32_t level = (us * 1953) / 1000; // 1us ≈ 1.953 tick
    pwm_set_chan_level(slice_num, channel, level);
}

int main() {
    stdio_init_all();

    // Init LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, true);

    // Init Switch (pakai pull-up karena switch ke GND)
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, false);
    gpio_pull_up(SW_PIN);

    // Init PWM untuk Servo
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel   = pwm_gpio_to_channel(SERVO_PIN);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, 39062); // 20ms (50Hz)
    pwm_init(slice_num, &config, true);

    while (1) {
        bool sw = !gpio_get(SW_PIN); // aktif LOW

        if (sw) { 
            set_servo_angle(slice_num, channel, 90); // Servo 90°
            gpio_put(LED_PIN, 1); // LED ON
        } else {
            set_servo_angle(slice_num, channel, 0);  // Servo 0°
            gpio_put(LED_PIN, 0); // LED OFF
        }

        sleep_ms(20);
    }
}
