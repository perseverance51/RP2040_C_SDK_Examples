/** c SDK v2.1 Build
 * @brief Is the RTC running
 * \ingroup hardware_rtc
 * 每间隔10秒钟触发一次报警，可重复触发
  本例程同RP2040_RTC
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/rtc.h"
#include "hardware/irq.h"
#include "pico/util/datetime.h"//datetime_to_str函数

#define BUILTIN_LED PICO_DEFAULT_LED_PIN
#define ALARM_AT_SECONDS 10
datetime_t t = {
    .year  = 2025,
    .month = 2,
    .day   = 21,
    .dotw  = 5, // 0 = Sunday, 1 = Monday, etc.
    .hour  = 11,
    .min   = 30,
    .sec   = 0
};
datetime_t alarmT = {
    .year  = -1,
    .month = -1,
    .day   = -1,
    .dotw  = -1, // 0 = Sunday, 1 = Monday, etc.
    .hour  = -1,
    .min   = -1,
    .sec   = 10
};
void rtc_alarm_callback(void) {
    printf("RTC Alarm triggered!\n");
    // 重新设置闹钟
   // rtc_get_datetime(&alarmT); // 获取当前时间
    alarmT.sec = (t.sec + ALARM_AT_SECONDS)%60;// 设置10秒后的时间
    rtc_set_alarm(&alarmT, rtc_alarm_callback); // 重新设置闹钟
}

int main()
{
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    stdio_init_all();
    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);
    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    // For more examples of clocks use see https://github.com/raspberrypi/pico-examples/tree/master/clocks

    rtc_init();
    rtc_set_datetime(&t);
    rtc_set_alarm(&alarmT, rtc_alarm_callback);
    irq_set_priority (RTC_IRQ, 1); //设置中断优先级
    irq_set_enabled(RTC_IRQ, true);
    while (true) {
        rtc_get_datetime(&t);
        datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
        printf("\r%s      ", datetime_str);
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
    }
}
