#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN 25

void setup()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

int main()
{
    setup();

    while(1){
        gpio_put(LED_PIN, 0); // Turn off the LED
        sleep_ms(1000);
        gpio_put(LED_PIN, 1); // Turn on the LED
        sleep_ms(1000);
    }

    return 0;
}
