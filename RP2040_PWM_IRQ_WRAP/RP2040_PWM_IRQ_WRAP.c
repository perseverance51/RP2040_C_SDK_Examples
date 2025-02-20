#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"

// Tell GPIO 6 and 7 they are allocated to the PWM
#define PWM_PIN 6
#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25
#define PWM_WRAP_VALUE 50000  // 计数器回绕值（每50ms回绕一次）
#define INTERRUPT_COUNT 10    // 500ms = 50ms * 10
volatile uint32_t interrupt_count = 0;  // 中断计数器

void pwm_isr()
{
    pwm_clear_irq(pwm_gpio_to_slice_num(PWM_PIN)); // 清除中断标志
    interrupt_count++;
    if (interrupt_count >= INTERRUPT_COUNT) {
        interrupt_count = 0;
        // 在这里添加500ms触发时的处理代码
       // printf("500ms interrupt triggered!\n");
       gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
    }
}
int main()
{
    stdio_init_all();
    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    printf("PWM 1MHz with 500ms interrupt example\n");

    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);  // 获取PWM片号

    pwm_config config = pwm_get_default_config();     // 获取默认配置
    //直接对已经初始化的 PWM 片进行时钟分频设置，会立即生效。
   // pwm_set_clkdiv(slice_num,125); // Set the clock divider to 125 (125MHz / 125 = 1MHz)
   pwm_config_set_clkdiv(&config, 125.0f);          // 设置分频器为125（125MHz / 125 = 1MHz）
    pwm_config_set_wrap(&config, PWM_WRAP_VALUE);               // 设置计数器回绕值
    pwm_init(slice_num, &config, true);               // 初始化PWM

    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);        // 设置GPIO为PWM功能
    // 设置占空比（50%）
    pwm_set_gpio_level(PWM_PIN, PWM_WRAP_VALUE / 2);
    // 配置PWM中断
    pwm_clear_irq(slice_num);                         // 清除中断标志
    pwm_set_irq_enabled(slice_num, true);             // 启用PWM中断
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_isr); // 设置中断处理函数
    irq_set_enabled(PWM_IRQ_WRAP, true);              // 启用PWM中断

    while (true)
    {
        puts("Hello, world!\n");
       // gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        sleep_ms(1000);
    }
}
