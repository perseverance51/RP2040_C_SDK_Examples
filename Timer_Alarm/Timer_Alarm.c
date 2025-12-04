#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/timer.h"
#include "hardware/clocks.h"

#define PLL_SYS_KHZ (133 * 1000)

int64_t alarm_callback(alarm_id_t id, void *user_data) {

    // Put your timeout handler code in here
    // 定时器触发时执行的任务
    printf("硬件定时器触发！时间: %lld ms\n", time_us_64() / 1000);

   // 返回2000000表示2秒后再次触发（2000毫秒 = 2000000微秒）
    return 2000000;
}



int main()
{
    // set sysclock
    set_sys_clock_khz(PLL_SYS_KHZ, true);
    stdio_init_all();
     gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_set_mask(1u << 25);
    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, true);
    // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

    while (true) {
       //板载LED状态翻转
        gpio_xor_mask(1u << 25);
        sleep_ms(1000);
    }
}
