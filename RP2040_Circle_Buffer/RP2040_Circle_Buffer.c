/**
 * SDK Version 2.1.0
 * @file RP2040_Circle_Buffer.c
 * 使用环形缓冲区存储数据
 * @author Perseverance51
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN        // 25号引脚

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define BUF_SIZE 256

uint8_t buf[BUF_SIZE];
volatile uint16_t buf_head = 0;
volatile uint16_t buf_tail = 0;

void on_uart_rx() {
    // 检查是否有数据可读
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        uint16_t next_head = (buf_head + 1) % BUF_SIZE;
        if (next_head != buf_tail) { // 检查缓冲区是否已满
            buf[buf_head] = ch;
            buf_head = next_head;
        }
    }
}
void print_buffer() {
    while (buf_tail != buf_head) { // 检查是否有数据需要处理
        uint8_t ch = buf[buf_tail]; // 从缓冲区读取一个字节
        buf_tail = (buf_tail + 1) % BUF_SIZE; // 更新尾指针
        uart_putc_raw(uart0, ch); // 打印字符
    }
    uart_putc(uart0, '\n'); // 打印换行符
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
    irq_set_priority(UART0_IRQ, 1);//设置中断优先级
    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));//需要初始化stdio_init_all()
    uart_puts(uart0, "UART0 Circle Buffer Test.\n");
    while (true) {
        // tight_loop_contents(); // 保持主循环运行
        if (buf_tail != buf_head) { // 检查是否有数据需要处理
            print_buffer();
        }
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
    }
}

