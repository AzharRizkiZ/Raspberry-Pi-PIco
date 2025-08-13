#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"

// Define constants ON OFF
#define OFF 0 // Define OFF
#define ON  1 // Define ON

// Messages for Core communication
#define LED_ON  1
#define LED_OFF 0

// Initialize contant
const unsigned char EXT_LED = 16; //External LED
const unsigned char INT_LED = 25; //Internal LED

// Global Variable
unsigned char gu1_signal;

//Basic Function
void basic_function() {
    gpio_init(EXT_LED);
    gpio_set_dir(EXT_LED, GPIO_OUT);

    while(1) {
        gpio_put(EXT_LED, ON);
        sleep_ms(1000); //1 Sec Delay
        gpio_put(EXT_LED, OFF);
        sleep_ms(1000); //1 Sec Delay
    }
}

// Core 1 Function
void core_1_function() {
    gpio_init(EXT_LED);
    gpio_set_dir(EXT_LED, GPIO_OUT);

    while (true) {
        // Wait for a signal from Core 0
        gu1_signal = multicore_fifo_pop_blocking();

        if (gu1_signal == LED_ON) {
            gpio_put(EXT_LED, ON);
        } else if (gu1_signal == LED_OFF) {
            gpio_put(EXT_LED, OFF);
        }
    }
}

void core_0_function() {
    // Init onboard LED for Core 0
    gpio_init(INT_LED);
    gpio_set_dir(INT_LED, GPIO_OUT);

    // Launch Core 1
    multicore_launch_core1(core_1_function);

    while(1) {
        // Core 0: turn ON onboard LED
        gpio_put(INT_LED, ON);

        // Signal Core 1 to turn ON external LED
        multicore_fifo_push_blocking(LED_ON);
        sleep_ms(1000); // Wait 1 second

        // Core 0: turn OFF onboard LED
        gpio_put(INT_LED, OFF);

        // Signal Core 1 to turn OFF external LED
        multicore_fifo_push_blocking(LED_OFF);
        sleep_ms(1000); // Wait 1 second
    }
}

void advanced_function() {
    core_0_function(); // Start Core 0 function
}

int main() {
    // Initialize standard I/O
    stdio_init_all();

    // Basic Function Call
    //basic_function();

    // Advanced Function Call
    advanced_function();
}
