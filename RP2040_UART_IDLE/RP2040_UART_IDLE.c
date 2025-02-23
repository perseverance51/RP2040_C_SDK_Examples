/**
 * @file RP2040_UART_IDLE.c
 * @brief UART 空闲检测
 * SDK: 2.1.0
 * author: Perseverance51
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/timer.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN        // 25号引脚

#define UART_ID     uart0
#define UART_IRQ     UART0_IRQ

#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART0_TX_PIN 0
#define UART0_RX_PIN 1

volatile bool uart_idle = false;

// UART 接收中断处理
void on_uart_rx() {

// 检查是否为空闲中断
    if (uart_get_hw(UART_ID)->mis & UART_UARTMIS_RTMIS_BITS) {
        // 清除空闲中断标志
        hw_clear_bits(&uart_get_hw(UART_ID)->icr, UART_UARTICR_RTIC_BITS);
        uart_idle = true;
        while (uart_is_readable(UART_ID)) {
            uint8_t ch = uart_getc(UART_ID);
            // 处理接收到的数据...
            uart_putc(UART_ID, ch);
        }
    }

}
// 初始化函数
void uart_setup() {
    gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);

    // Initialize our UARTs.
    uart_init(UART_ID, BAUD_RATE);
    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);
    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    // Turn on the FIFOs
    uart_set_fifo_enabled(UART_ID, true);

    // 启用硬件空闲检测
    hw_set_bits(&uart_get_hw(UART_ID)->imsc, UART_UARTIMSC_RTIM_BITS);
//hw_set_bits(&uart_get_hw(uart0)->imsc, UART_UARTIMSC_RXIM_BITS);//启用UART接收中断
    // 启用接收中断
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    uart_set_irq_enables(UART_ID, true, false);
    irq_set_enabled(UART_IRQ, true);
}

int main()
{
    stdio_init_all();
    uart_setup();
    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));

    while (true) {

        //  sleep_ms(1000);
        if (uart_idle) {
            uart_idle = false;
            // 处理接收空闲事件...

            gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        }
    }
}
