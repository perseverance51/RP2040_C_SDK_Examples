/*
busy_wait_ms 函数:
    该函数用于阻塞当前线程，等待指定的毫秒数。精确延时，硬件时序
     参数:
        ms: 要等待的毫秒数
        单位: 毫秒
busy_wait_until 函数:
    该函数用于阻塞当前线程，直到指定的时间点到达。100%占用	精确时间同步
     参数:
        target_time: 目标时间点
        单位: 绝对时间（absolute_time_t）
sleep_ms 函数:
    该函数用于阻塞当前线程，等待指定的毫秒数。节省功耗
     参数:
        ms: 要等待的毫秒数
        单位: 毫秒

*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#define PLL_SYS_KHZ (133 * 1000)

int main() {

    set_sys_clock_khz(PLL_SYS_KHZ, true);
    stdio_init_all();
// 初始化GPIO 25为输出模式
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    printf("Demo busy_wait_until...\n");

    // 获取当前时间
    absolute_time_t current_time = get_absolute_time();

    // 计算1秒后的时间点
    absolute_time_t target_time = delayed_by_ms(current_time, 1000);

    printf("Wait until the specified time point...\n");

    // 等待直到目标时间点
    busy_wait_until(target_time);

    printf("Reach the target time point!\n");

    // 另一种用法：精确控制LED闪烁
    while (true) {
        absolute_time_t next_toggle = get_absolute_time();
        next_toggle = delayed_by_ms(next_toggle, 250); // 250ms后

        gpio_put(25, 1);  // LED亮
        busy_wait_until(next_toggle);

        next_toggle = delayed_by_ms(next_toggle, 250); // 再250ms后
        gpio_put(25, 0);  // LED灭
        busy_wait_until(next_toggle);
    }

    return 0;
}
