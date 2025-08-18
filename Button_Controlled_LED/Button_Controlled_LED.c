#include "pico/stdlib.h"

#define LED_PIN 16       // LED eksternal
#define SWITCH_PIN 15    // Switch geser

int main() {
    stdio_init_all();

    // Inisialisasi pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);

    while (true) {
        // Baca posisi switch
        int sw = gpio_get(SWITCH_PIN);

        // Jika switch ke GND -> LED ON
        if (sw == 0) {
            gpio_put(LED_PIN, 1);
        } else {
            gpio_put(LED_PIN, 0);
        }

        sleep_ms(10); // debounce ringan (jeda sebelum nyala/mati 0.01 detik)
    }
}
