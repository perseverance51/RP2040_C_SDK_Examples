/*
  CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program WouoUIPage.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program WouoUIPage.elf verify reset exit"
 */
#include <stdio.h>
#include "string.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "TestUI.h"

#define LED1 25
#define LED2 6

uint8_t rxBuffer;
static const char TARGET_CHARS[] = "abcd";

void uart0ISR(void)
{
    while (uart_is_readable(uart0))
    {
        uint8_t ch = uart_getc(uart0);

            if (strchr(TARGET_CHARS, ch) != NULL) {
                rxBuffer = ch; // 保存接收到的字符
            }

        // if (uart_is_writable(uart0)) // 确定TX FIFO中是否有可用空间。
        // {
        //     // uart_putc(uart0, ch);//转发数据
        //     uart_putc_raw(uart0, ch); // 同上，需要使能fifo功能才可以使用
        // }
    }
}

static void configure_gpio(void)
{

    gpio_init(LED1);
    gpio_init(LED2);
    gpio_set_dir(LED1, GPIO_OUT);
    gpio_set_dir(LED2, GPIO_OUT);

    gpio_put(LED1, 0);
    gpio_put(LED2, 0);

}

int main()
{
    stdio_init_all();
    uart_set_hw_flow(uart0, false, false); // 关闭硬件流控
    uart_set_fifo_enabled(uart0, true);    // 开启缓存
    hw_write_masked(&uart_get_hw(uart0)->ifls, 0b100 << UART_UARTIFLS_RXIFLSEL_LSB,
                    UART_UARTIFLS_RXIFLSEL_BITS);   // 设置接收中断的触发条件为FIFO不为空
    irq_set_exclusive_handler(UART0_IRQ, uart0ISR); // 配置中断回调
    irq_set_enabled(UART0_IRQ, true);               // 开启串口中断
    uart_set_irq_enables(uart0, true, false);       // 开启串口1中断,接收中断，关闭发送中断
    irq_set_priority(UART0_IRQ, 1);                 // 设置中断优先级
    configure_gpio();
    TestUI_Init();
    OLED_operate_gram(PEN_CLEAR); // 清缓存
    uart_puts(uart0,"Hello, WouoUIPage");
    //    puts("Hello, world!");

    while (1)
    {
        TestUI_Proc();
        // 检查串口是否有数据可读
       // uart_read_blocking(uart0, &rxBuffer, 1); // 读取一个字符
                                                 // rxBuffer = uart_getc(uart0);
       // if ((rxBuffer > 0x60) && (rxBuffer < 0x65))
       if (rxBuffer != 0)
        { // 只处理abcd字符
            switch (rxBuffer)
            { // 没有按键，使用串口代替
            case 'a':
                OLED_MsgQueSend(msg_click);
                break; // 单击
            case 'b':
                OLED_MsgQueSend(msg_up);
                break; // 上翻页
            case 'c':
                OLED_MsgQueSend(msg_down);
                break; // 下翻页
            case 'd':
                OLED_MsgQueSend(msg_return);
                break; // 返回
            default:
                break;
            }

            printf("Received: %c\n", rxBuffer);
            rxBuffer = 0;
            fflush(stdout);//强制刷新缓冲区
        }
        int item_max_value1 = setting_option_array[3].val;
        item_max_value1 == 0?gpio_put(LED1, 0):gpio_put(LED1, 1);
        int item_max_value2 = setting_option_array[4].val;
        item_max_value2 == 0?gpio_put(LED2, 0):gpio_put(LED2, 1);
    }

    return 0;
}
