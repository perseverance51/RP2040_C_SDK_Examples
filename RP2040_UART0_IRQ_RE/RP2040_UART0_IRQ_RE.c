/*
 SDK v2.1.0
 RP2040_UART0_IRQ_RE.c
 2023.03.14
    串口接收中断
    串口回显
*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

void on_uart_rx() {
    // 检查是否有数据可读
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID); // 读取一个字节
        // 处理接收到的数据（例如打印或存储）
        //printf("Received: %c\n", ch);
        uart_putc(UART_ID, ch); // 回显接收到的数据
    }
}

int main()
{
    stdio_init_all();
    // 初始化UART
    uart_init(UART_ID, BAUD_RATE);

    // 设置UART引脚
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // 配置UART参数
    uart_set_hw_flow(UART_ID, false, false); // 禁用硬件流控
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // 启用UART接收中断
    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx); // 设置中断处理函数
    irq_set_enabled(UART0_IRQ, true);                 // 启用UART0中断
    uart_set_fifo_enabled(UART_ID, true); // 启用FIFO
    uart_set_irq_enables(UART_ID, true, false);       // 启用接收中断，禁用发送中断
    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));//需要初始化stdio_init_all()
    while (true) {
        // tight_loop_contents(); // 保持主循环运行
        //    uart_puts(uart0,"Perseverance51!\n");
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
    }
}
