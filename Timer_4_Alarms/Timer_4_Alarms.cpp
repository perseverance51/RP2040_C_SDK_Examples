/*
程序说明：
本程序演示RP2040的4个硬件定时器报警同时使用
每个定时器有不同的触发间隔和独立功能
*/
#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#define PLL_SYS_KHZ (133 * 1000)

// 存储4个定时器的ID
alarm_id_t timer_ids[4];

// 定时器0回调 - 快速闪烁 (500ms)
int64_t alarm_callback_0(alarm_id_t id, void *user_data) {
    printf("定时器0触发 - 快速任务\n");
    // 执行快速任务，如LED快速闪烁
    return 500000; // 500ms后再次触发
}

// 定时器1回调 - 中速任务 (1秒)
int64_t alarm_callback_1(alarm_id_t id, void *user_data) {
    printf("定时器1触发 - 中速任务\n");
    // 执行中速任务，如数据采集
    return 1000000; // 1秒后再次触发
}

// 定时器2回调 - 慢速任务 (2秒)
int64_t alarm_callback_2(alarm_id_t id, void *user_data) {
    printf("定时器2触发 - 慢速任务\n");
    // 执行慢速任务，如状态报告
    return 2000000; // 2秒后再次触发
}

// 定时器3回调 - 超慢任务 (5秒)
int64_t alarm_callback_3(alarm_id_t id, void *user_data) {
    printf("定时器3触发 - 超慢任务\n");
    // 执行超慢任务，如系统检查
    return 5000000; // 5秒后再次触发
}

int main() {
    
    set_sys_clock_khz(PLL_SYS_KHZ, true);
    stdio_init_all();

    // 初始化GPIO
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // 创建4个定时器
    timer_ids[0] = add_alarm_in_ms(500, alarm_callback_0, NULL, true);
    timer_ids[1] = add_alarm_in_ms(1000, alarm_callback_1, NULL, true);
    timer_ids[2] = add_alarm_in_ms(2000, alarm_callback_2, NULL, true);
    timer_ids[3] = add_alarm_in_ms(5000, alarm_callback_3, NULL, true);

    printf("4个定时器已启动！\n");

    // 主循环可以执行其他任务
    while (true) {
        // 主循环任务，如LED心跳指示
        gpio_xor_mask(1u << 25);
        sleep_ms(100);
    }
}
