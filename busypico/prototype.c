#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// Pin Definitions
#define SPI_PORT spi0
#define PIN_MISO 0      // Not used, but defined for SPI init
#define PIN_SCK  18     // 74HC595 SHCP (Clock)
#define PIN_MOSI 19     // 74HC595 DS (Data)
#define PIN_CS   17     // 74HC595 STCP (Latch)

void pulse_latch() {
    gpio_put(PIN_CS, 0);
    sleep_us(1);
    gpio_put(PIN_CS, 1);
    sleep_us(1);
    gpio_put(PIN_CS, 0);
}

int main() {
    stdio_init_all();

    spi_init(SPI_PORT, 1000 * 1000);

    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 0);

    uint8_t led_pattern = 0x01;

    printf("Starting SPI LED sequence...\n");

    while (true) {
        spi_write_blocking(SPI_PORT, &led_pattern, 1);

        pulse_latch();

        printf("Pattern sent: 0x%02X\n", led_pattern);

        if (led_pattern == 0x80) {
            led_pattern = 0x01;
        } else {
            led_pattern <<= 1;
        }

        sleep_ms(250);
    }

    return 0;
}
