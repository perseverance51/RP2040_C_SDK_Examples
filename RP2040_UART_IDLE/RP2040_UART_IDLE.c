/**
 * @file RP2040_UART_IDLE.c
 * @brief UART 空闲检测
 * SDK: 2.1.0
 * author: Perseverance51
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN        // 25号引脚

#define UART_ID     uart0
#define UART_IRQ     UART0_IRQ

#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART0_TX_PIN 0
#define UART0_RX_PIN 1

//软件方式定时器定时空闲检测
//#define SOFT_UART_IDLE
//硬件方式空闲检测
#define HARD_UART_IDLE

volatile bool uart_idle = false;
#ifdef SOFT_UART_IDLE
// 全局变量
alarm_id_t uart_idle_alarm = -1;
// 定义空闲时间阈值（单位：微秒）
#define UART_IDLE_TIMEOUT 1000
// 定时器回调函数
int64_t uart_idle_alarm_callback(alarm_id_t id, void *user_data) {
    uart_idle = true;
    return 0;  // 不重复定时器
}
#endif
// UART 接收中断处理
void on_uart_rx() {

#ifdef SOFT_UART_IDLE
    // 软件方式检测空闲

    //每次收到数据时重置定时器
    if (uart_idle_alarm >= 0) {
        cancel_alarm(uart_idle_alarm);
    }
    uart_idle_alarm = add_alarm_in_us(UART_IDLE_TIMEOUT, uart_idle_alarm_callback, NULL, false);

#endif

#ifdef HARD_UART_IDLE
// 检查是否为空闲中断
    if (uart_get_hw(UART_ID)->mis & UART_UARTMIS_RTMIS_BITS) {
        // 清除空闲中断标志
        hw_clear_bits(&uart_get_hw(UART_ID)->icr, UART_UARTICR_RTIC_BITS);
        uart_idle = true;
    }
#endif
    // 读取数据
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        // 处理接收到的数据...
        uart_putc(UART_ID, ch);
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

        sleep_ms(1000);
        if (uart_idle) {
            uart_idle = false;
            // 处理接收空闲事件...
            gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        }
    }
}
