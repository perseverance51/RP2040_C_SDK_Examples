#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"

//频率参数
#define PLL_SYS_KHZ (133 * 1000)
#define BUILTIN_LED PICO_DEFAULT_LED_PIN    // LED is on the same pin as the default LED 25

// 定义要采样的4个通道
#define ADC_CHANNEL_0 0  // 对应GPIO26
#define ADC_CHANNEL_1 1  // 对应GPIO27
#define ADC_CHANNEL_2 2  // 对应GPIO28
#define ADC_CHANNEL_3 3  // 对应GPIO29
#define ADC_CHANNEL_MASK 0x0F  // 0b1111，启用通道0-3


int main()
{
    set_sys_clock_khz(PLL_SYS_KHZ, true);
    stdio_init_all();
     gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    adc_init();
    // 3. 配置外部GPIO为ADC输入模式（禁用GPIO数字功能）
    adc_gpio_init(26);  // 通道0对应GPIO26
    adc_gpio_init(27);  // 通道1对应GPIO27
    adc_gpio_init(28);  // 通道2对应GPIO28
    adc_gpio_init(29);  // 通道3对应GPIO29

    // 4. 启用ADC轮询采样，设置掩码为0x0F（通道0-3）
    adc_set_round_robin(ADC_CHANNEL_MASK);


    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    printf("ADC 4 channels round-robin sampling\n");
    while (true) {

        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
         // 存储4个通道的采样值
        uint16_t adc_values[4] = {0};
        uint8_t current_channel;

        // 依次读取4个通道的轮询数据
        for (int i = 0; i < 4; i++) {
            // 读取当前通道的ADC值（12位精度，范围0-4095）
            adc_values[i] = adc_read();
            // 获取当前采样的通道编号
            current_channel = adc_get_selected_input();
            printf("通道%d: ADC原始值=%d | 电压=%.3fV\r\n",
                   current_channel,
                   adc_values[i],
                   adc_values[i] * 3.3f / 4095.0f);  // 3.3V为参考电压
        }
    }
}
