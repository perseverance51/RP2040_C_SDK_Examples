#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "hardware/irq.h"

//频率参数
#define PLL_SYS_KHZ (133 * 1000)
#define BUILTIN_LED PICO_DEFAULT_LED_PIN    // LED is on the same pin as the default LED 25

// 定义ADC相关参数
#define ADC_CHANNEL_MASK 0x0F  // 启用通道0-3的轮询采样
#define FIFO_THRESHOLD 4       // FIFO数据达到4个时触发DREQ/中断（每个通道一个样本）
#define SAMPLE_BUFFER_SIZE 16  // 采样数据缓存区大小

// 全局变量：存储采样数据和计数
static uint16_t adc_sample_buffer[SAMPLE_BUFFER_SIZE] = {0};
static volatile uint8_t buffer_index = 0;
static volatile bool data_ready = false;  // 数据就绪标志


/**
 * @brief ADC中断服务函数：读取FIFO数据并缓存
 */
static void adc_irq_handler(void) {
    // 清空ADC中断标志
    irq_clear(ADC_IRQ_FIFO);
    uint8_t fifo_level = adc_fifo_get_level();
   // printf("FIFO Level: %d\r\n", fifo_level);
   if (fifo_level == 0) {
        return;
    }

    if (fifo_level >= FIFO_THRESHOLD) {
       for (buffer_index = 0; buffer_index < SAMPLE_BUFFER_SIZE; buffer_index++)
            {adc_sample_buffer[buffer_index] = adc_fifo_get();}
            // 缓冲区满时设置数据就绪标志
            data_ready = true;
    }
}


int main()
{
    stdio_init_all();

    // 等待串口初始化完成
    sleep_ms(2000);

    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));
    printf("RP2040 ADC 轮询+FIFO+中断采样示例\r\n");
    printf("采样通道：0-3（GPIO26-29） | FIFO阈值：%d\r\n", FIFO_THRESHOLD);
    printf("-----------------------------\r\n");

    // 初始化LED
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, GPIO_OUT);
   // gpio_pull_up(BUILTIN_LED);
    // 初始化ADC模块
    adc_init();

    // 配置外部GPIO为ADC输入（禁用数字功能）
    adc_gpio_init(26);  // 通道0
    adc_gpio_init(27);  // 通道1
    adc_gpio_init(28);  // 通道2
    adc_gpio_init(29);  // 通道3

    // 配置ADC FIFO
    adc_fifo_setup(
        true,                // 启用FIFO
        false,                // 启用DREQ（配合中断）
        FIFO_THRESHOLD,      // FIFO数据达到4个时触发中断
        false,               // 不将错误信息存入FIFO
        false                // 不进行字节移位（保持12位原始数据）
    );

    // 启用ADC轮询采样（通道0-3）
    adc_set_round_robin(ADC_CHANNEL_MASK);

    // 配置ADC中断
  //   irq_set_priority(ADC_IRQ_FIFO, PICO_HIGHEST_IRQ_PRIORITY); // Set the priority of the ADC IRQ
    irq_set_exclusive_handler(ADC_IRQ_FIFO, adc_irq_handler);  // 绑定中断服务函数
    adc_irq_set_enabled(true);                                // 启用ADC中断
    irq_set_enabled(ADC_IRQ_FIFO, true);                      // 启用NVIC中的ADC中断

    // 启动ADC自由运行模式（持续采样）
    adc_run(true);

    printf("ADC采样已启动，开始采集数据...\r\n");

    uint32_t loop_count = 0;

    while (true) {

        gpio_xor_mask(1u << BUILTIN_LED);
        sleep_ms(500);

// /*
        // 检查是否有数据需要处理
        if (data_ready) {
            printf("采集到 %d 个样本：\r\n", buffer_index);

            // 遍历缓存区数据并解析通道和电压
            for (uint8_t i = 0; i < buffer_index; i++) {
                // ADC采样值的高3位为通道编号，低12位为采样数据
                uint8_t channel = (adc_sample_buffer[i] >> 12) & 0x07;
                uint16_t adc_raw = adc_sample_buffer[i] & 0xFFF;
                float voltage = adc_raw * 3.3f / 4095.0f;  // 3.3V参考电压

                printf("通道%d | ADC值：%4d | 电压：%.3fV\r\n", channel, adc_raw, voltage);
            }
            printf("-----------------------------\r\n");

            // 重置缓冲区和标志
            buffer_index = 0;
            data_ready = false;
        }
// */

    }
}
