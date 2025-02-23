/**
 * library from:https://github.com/dgatf/Pio-UART-RX-RP2040/tree/master
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "uart_rx.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN        // 25号引脚
#define RX_PIN 1    // 1号引脚,串口接收引脚

volatile uint8_t Rx_IRQ_FLAG = 0;

void rx_handler(void) {
    Rx_IRQ_FLAG = 1;
}


int main()
{
    stdio_init_all();
    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);

    PIO pio = pio0;         // values: pio0, pio1
    uint pin = RX_PIN;           // rx pin. Any gpio is valid
    uint irq = PIO0_IRQ_0;  // values for pio0: PIO0_IRQ_0, PIO0_IRQ_1. values for pio1: PIO1_IRQ_0, PIO1_IRQ_1
    uint baudrate = 115200;
    uart_rx_init(pio, pin, baudrate);
    uart_rx_set_handler(rx_handler, irq);

    while (true) {
        //printf("Hello, world!\n");
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        if (Rx_IRQ_FLAG) {
            Rx_IRQ_FLAG = 0;
            while (uart_rx_available()) printf("%c", uart_rx_read());
        }

    }
}
