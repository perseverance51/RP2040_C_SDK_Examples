/*
  CMSIS-DAP: openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program Watchdog.elf verify reset exit"

 jlink: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program Watchdog.elf verify reset exit"
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25

void watchdog_disable(void)
{
    hw_clear_bits(&watchdog_hw->ctrl, WATCHDOG_CTRL_ENABLE_BITS);
}

int main()
{
    //设置系统时钟
   // set_sys_clock_khz(133000, true);
    // Initialize the stdio UART
    stdio_init_all();
    // 打印系统时钟频率
    //  uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    printf("System clock speed: %d Hz\n", clock_get_hz(clk_sys));
    //参考时钟频率
    printf("Watchdog clock speed: %d Hz\n", clock_get_hz(clk_ref));
    // 打印参考时钟源
    uint32_t clk_ref_source = clocks_hw->clk[clk_ref].ctrl & CLOCKS_CLK_REF_CTRL_SRC_BITS;
    if (clk_ref_source == CLOCKS_CLK_REF_CTRL_SRC_VALUE_ROSC_CLKSRC_PH) {
        printf("clk_ref source: ROSC\n");
    } else if (clk_ref_source == CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC) {
        printf("clk_ref source: XOSC\n");
    }

    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    // Watchdog example code
    if (watchdog_caused_reboot())
    {
        // Whatever action you may take if a watchdog caused a reboot
        // printf("Watchdog caused a reboot\n");
        // 检查是否由看门狗引起的重启
        printf("系统因看门狗超时重启！\n");
    }
    else
    {
        printf("正常启动\n");
    }

    // Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code当调试器逐步执行代码时，看门狗是否应该暂停
    watchdog_enable(600, 1); // 启用看门狗，并设置超时时间:/ms

    // You need to call this function at least more often than the 100ms in the enable call to prevent a rebootwatchdog_update();
    for (uint i = 0; i < 5; i++)
    {
        printf("Updating watchdog %d\n", i);
        sleep_ms(500);
        watchdog_update();                 // 更新看门狗
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        __asm volatile("nop\n");
    }
    // Wait in an infinite loop and don't update the watchdog so it reboots us
    // printf("Waiting to be rebooted by watchdog\n");
    watchdog_disable(); // 禁用看门狗
    // 验证看门狗是否已禁用
    if (!(watchdog_hw->ctrl & WATCHDOG_CTRL_ENABLE_BITS))
    {
        printf("看门狗已成功禁用\n");
    }
    printf("Watchdog clock speed: %d Hz\n", clock_get_hz(clk_ref));
    uint32_t max_time_ms = (0xffffffu * 1000) / (clock_get_hz(clk_ref) / 2);
printf("Maximum watchdog timeout: %d ms\n", max_time_ms);

    watchdog_enable(2200, 1); // 启用看门狗，并设置超时时间:/m    s

    while (1)
    {
//打印看门狗的当前计数值
       printf("Watchdog count1: %d\n", watchdog_get_count());
          // 打印系统时钟频率
 //   printf("System clock speed: %d Hz\n", clock_get_hz(clk_sys));
    //看门狗时钟
   // printf("Watchdog clock speed: %d Hz\n", clock_get_hz(clk_ref));
        sleep_ms(250);
        printf("Watchdog count2: %d\n", watchdog_get_count());
         sleep_ms(250);
        // gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        // __asm volatile("nop\n");
         watchdog_update();
    }

    // puts("Hello, world!");

    // return 0;
}
