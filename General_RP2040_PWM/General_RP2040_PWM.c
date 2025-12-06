/*
 程序说明：通用PWM输出：
 PWM 输出占空比可通过串口输入调整。

 串口输入ASCII码0-255，对应占空比0-100%。（注意：输入的ASCII码必须为数字字符，不能为字母或特殊字符，不要勾选回车换行符）
 PWM波形：
  ┌─────────────────────────────────────────────────────────┐
  │                                                         │
  │ 高电平(50%占空比示例):                                    │
  │  ┌───┐     ┌───┐     ┌───┐     ┌───┐     ┌───┐          │
  │  │   │     │   │     │   │     │   │     │   │          │
  │──┘   └─────┘   └─────┘   └─────┘   └─────┘   └──────────│
  │                                                         │
  │ 高电平(25%占空比示例):                                    │
  │  ┌─┐   ┌─┐   ┌─┐   ┌─┐   ┌─┐   ┌─┐   ┌─┐   ┌─┐          │
  │  │ │   │ │   │ │   │ │   │ │   │ │   │ │   │ │          │
  │──┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └──────────│
  │                                                         │
  │ 高电平(75%占空比示例):                                    │
  │  ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐        │
  │  │     │ │     │ │     │ │     │ │     │ │     │        │
  │──┘     └─┘     └─┘     └─┘     └─┘     └─┘     └────────│
  │                                                         │
  └─────────────────────────────────────────────────────────┘

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

// PWM配置
#define PWM_PIN 2        // 使用GPIO 2作为PWM输出
#define PWM_FREQUENCY 1000  // PWM频率 1kHz
#define PWM_WRAP 255     // PWM计数器最大值

// 串口缓冲区大小
#define BUFFER_SIZE 32

int main()
{
    stdio_init_all();

    printf("RP2040 PWM占空比调节程序\n");
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));

    // 初始化PWM
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    uint channel = pwm_gpio_to_channel(PWM_PIN);

    // 配置PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);  // 系统时钟分频
    pwm_config_set_wrap(&config, PWM_WRAP);   // 设置计数器最大值
    pwm_init(slice_num, &config, true);       // 启动PWM

    printf("PWM初始化完成，开始等待串口输入...\n");
    printf("请输入占空比数值 (0-255):\n");

    char buffer[BUFFER_SIZE];
    uint16_t current_duty_cycle = 128;  // 默认占空比50%

    // 设置初始占空比
    pwm_set_chan_level(slice_num, channel, current_duty_cycle);
    printf("当前占空比: %d/%d (%.1f%%)\n", current_duty_cycle, PWM_WRAP,
           (current_duty_cycle * 100.0) / PWM_WRAP);

    while (true) {
        // 检查串口是否有数据输入
        int c = getchar_timeout_us(1000); // 等待1ms
        if (c != PICO_ERROR_TIMEOUT) {
            // 手动构建输入字符串
            int index = 0;
            buffer[index++] = (char)c;

            // 继续读取直到遇到换行符或缓冲区满
            while (index < BUFFER_SIZE - 1) {
                c = getchar_timeout_us(1000); // 等待1ms
                if (c == PICO_ERROR_TIMEOUT) {
                    break;
                }
                if (c == '\r' || c == '\n') {
                    break;
                }
                buffer[index++] = (char)c;
            }
            buffer[index] = '\0'; // 字符串结束符

            printf("接收到输入: [%s]\n", buffer); // 调试信息

            // 去除前导空格
            char *input_str = buffer;
            while (*input_str == ' ' || *input_str == '\t') {
                input_str++;
            }

            // 检查输入是否为空
            if (strlen(input_str) == 0) {
                printf("错误: 输入不能为空\n");
                printf("请输入新的占空比数值 (0-255):\n");
                continue;
            }

            // 尝试将输入转换为数字
            char *endptr;
            long new_duty_cycle = strtol(input_str, &endptr, 10);

            // 检查转换是否成功
            if (endptr == input_str) {
                printf("错误: 输入不是有效的数字\n");
            } else if (*endptr != '\0') {
                printf("错误: 输入包含非数字字符 '%c'\n", *endptr);
            } else if (new_duty_cycle < 0 || new_duty_cycle > PWM_WRAP) {
                printf("错误: 请输入0-255之间的有效数字\n");
            } else {
                current_duty_cycle = (uint16_t)new_duty_cycle;

                // 更新PWM占空比
                pwm_set_chan_level(slice_num, channel, current_duty_cycle);

                printf("占空比已更新: %d/%d (%.1f%%)\n", current_duty_cycle, PWM_WRAP,
                       (current_duty_cycle * 100.0) / PWM_WRAP);
            }

            printf("请输入新的占空比数值 (0-255):\n");
        }

        // 短暂延时，避免过度占用CPU
        sleep_ms(50);
    }
}
