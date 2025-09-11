#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200

// Pin Pico
#define UART_TX_PIN 0  // GP0 → ESP8266 RX
#define UART_RX_PIN 1  // GP1 ← ESP8266 TX

int main() {
    stdio_init_all();

    // Init UART
    uart_init(UART_ID, BAUD_RATE);

    // Set pin fungsi ke UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Kirim perintah AT sekali
    uart_puts(UART_ID, "AT\r\n");

    // Tunggu balasan dari ESP8266
    char c;
    printf("Menunggu balasan ESP8266...\n");
    sleep_ms(100); // delay kecil

    while (uart_is_readable(UART_ID)) {
        c = uart_getc(UART_ID);
        putchar(c);  // tampilkan di Serial Monitor USB
    }

    printf("\nSelesai.\n");

    return 0;  // program selesai tanpa loop
}