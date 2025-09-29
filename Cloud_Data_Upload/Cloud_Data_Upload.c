#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define TX_PIN 0
#define RX_PIN 1

int main() {
    stdio_init_all();

    // Inisialisasi UART untuk komunikasi ke ESP8266
    uart_init(UART_ID, 115200);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);

    // Inisialisasi ADC untuk sensor suhu internal
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);  // channel 4 = internal temperature sensor

    while (true) {
        uint16_t raw = adc_read();
        float voltage = raw * 3.3f / (1 << 12); // 12-bit ADC
        float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;

        // Kirim data suhu ke ESP8266 via UART
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "CPU Temp: %.2f C\r\n", temperature);
        uart_puts(UART_ID, buffer);

        // Debug ke USB Serial Monitor (jika mau)
        printf("Sent: %s", buffer);

        sleep_ms(2000);
    }
}
