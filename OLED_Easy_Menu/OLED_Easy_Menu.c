#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "oled.h"
#include "pico/binary_info.h"

#include "menu_navigator.h"
#include "menu_wrapper.h"
#include "generated_header.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
//#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

static const char TARGET_CHARS[] = "abcd";

// 1. 创建导航器
void* navigator;

uint8_t rxBuffer;   // 接收缓存

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

int main()
{
    // 2. 获取主菜单项
navigator = menu_builder(getMainItem());
	uint8_t key;
    stdio_init_all();
    uart_set_hw_flow(uart0, false, false); // 关闭硬件流控
    uart_set_fifo_enabled(uart0, true);    // 开启缓存
    hw_write_masked(&uart_get_hw(uart0)->ifls, 0b100 << UART_UARTIFLS_RXIFLSEL_LSB,
                    UART_UARTIFLS_RXIFLSEL_BITS);   // 设置接收中断的触发条件为FIFO不为空
    irq_set_exclusive_handler(UART0_IRQ, uart0ISR); // 配置中断回调
    irq_set_enabled(UART0_IRQ, true);               // 开启串口中断
    uart_set_irq_enables(uart0, true, false);       // 开启串口1中断,接收中断，关闭发送中断
    irq_set_priority(UART0_IRQ, 1);                 // 设置中断优先级
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
     // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(I2C_SDA, I2C_SCL, GPIO_FUNC_I2C));
    OLED_Init();
    OLED_PrintASCIIString(0,0,"OLED Easy_Menu",&afont8x6,0);
    OLED_ShowFrame();
     sleep_ms(1000);

    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c
 printf("OLED Easy_Menu\n");
    while (true) {
        if((rxBuffer > 0x60) && (rxBuffer < 0x65)) {  //只处理abcd字符
            switch(rxBuffer) { //没有按键，使用串口代替
                case 'a':
                    key = UP;
                    break; //单击
                case 'b':
                    key = DOWN;
                    break; //上翻页
                case 'c':
                    key = LEFT;
                    break; //下翻页
                case 'd':
                    key = RIGHT;
                    break; //返回
                default:
                    break;
            }
            rxBuffer = 0;
					}
// 用户实现的按键获取函数
		menu_handle_input(navigator, key);
					key = NONE;

		// 刷新显示菜单
		menu_display(navigator);


        sleep_ms(50);
    }
    // 3. 清理资源
	menu_delete(navigator);
}


