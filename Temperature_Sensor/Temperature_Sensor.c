#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Konstanta dari datasheet RP2040 section 4.9.5
const float conversion_factor = 3.3f / (1 << 12); // 12-bit ADC
// Rumus suhu internal: T = 27 - (ADC_voltage - 0.706)/0.001721

int main() {
    stdio_init_all();

    // Inisialisasi ADC
    adc_init();
    adc_set_temp_sensor_enabled(true); // mengaktifkan sensor suhu internal
    adc_select_input(4); // ADC input 4 = sensor suhu internal

    while (true) {
        uint16_t raw = adc_read(); //Membaca nilai ADC
        float voltage = raw * conversion_factor;
        float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;

        printf("Temperature: %.2f°C\n", temperature);

        sleep_ms(1000); // update setiap 1 detik
    }
}

// akses PuttY
// Pilih Serial Line berdasarkan device Manager
// Taruh Speednya 115200