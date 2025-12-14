#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"

//频率参数
#define PLL_SYS_KHZ (133 * 1000)
#define BUILTIN_LED PICO_DEFAULT_LED_PIN    // LED is on the same pin as the default LED 25

#define ADC_TEMP    4

float read_temperature() {
    // 选择温度传感器通道
    adc_select_input(ADC_TEMP);

    // 启动单次转换
 //   adc_run_once(true);

    // 等待转换完成
    // while (!adc_fifo_is_empty()) {
    //     uint16_t raw = adc_fifo_get();
// 读取12位ADC值（Pico SDK默认12位采样）
        uint16_t adc_raw = adc_read();
        // 换算为电压（3.3V参考，12位范围0-4095）
        float voltage = adc_raw * 3.3f / 4095.0f;
        // 计算温度
        float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;
        // 打印结果
        printf("ADC原始值: %d, 电压: %.3fV, 内部温度: %.2f℃\n", adc_raw, voltage, temperature);
        // 根据 RP2040 数据手册公式转换温度
        // T = 27 - (ADC_voltage - 0.706)/0.001721
        // float voltage = raw * 3.3f / 4096.0f;
        // float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;

        return temperature;
  //  }

    return -273.15f;  // 错误值
}

int main()
{
    set_sys_clock_khz(PLL_SYS_KHZ, true);
    stdio_init_all();
    adc_init();
    // 启用温度传感器
    adc_set_temp_sensor_enabled(true);
    // 选择ADC通道4
    adc_select_input(4);
     gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));
    printf("ADC4  internal temperature sensor\n");


    while (true) {

         gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
         float temperature = read_temperature();
        printf("Temperature: %.2f °C\n", temperature);
        sleep_ms(1000);
    }
}
