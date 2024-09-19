/*
 CMSIS-DAP烧录命令：openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c  "adapter speed 5000"-c "program RP2040_Timer.elf verify reset exit"

 jlink命令: openocd -f interface/jlink.cfg -f target/rp2040.cfg  -c  "adapter speed 2000" -c  "program RP2040_Timer.elf verify reset exit"


*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#define BUILTIN_LED PICO_DEFAULT_LED_PIN // LED is on the same pin as the default LED 25

volatile bool timer_fired = false;

int64_t alarm_callback(alarm_id_t id, void *user_data)
{
    printf("Timer %d fired!\n", (int)id);
    timer_fired = true;
    // Can return a value here in us to fire in the future
    return 2000 * 2000;
}

// This is a repeating timer callback that will be called every 500ms
bool repeating_timer_callback(struct repeating_timer *t)
{
    printf("Repeat at %lld\n", time_us_64());
    return true;
}
// Simplest form of getting 64 bit time from the timer.
// It isn't safe when called from 2 cores because of the latching
// so isn't implemented this way in the sdk
static uint64_t get_time(void) {
    // Reading low latches the high value
    uint32_t lo = timer_hw->timelr;
    uint32_t hi = timer_hw->timehr;
    return ((uint64_t) hi << 32u) | lo;
}
/// \end::get_time[]

/// \tag::alarm_standalone[]
// Use alarm 0
#define ALARM_NUM 0
#define ALARM_IRQ TIMER_IRQ_0

// Alarm interrupt handler
static volatile bool alarm_fired;

static void alarm_irq(void) {
    // Clear the alarm irq
    hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM);

    // Assume alarm 0 has fired
    printf("Alarm IRQ fired\n");
    alarm_fired = true;
}

static void alarm_in_us(uint32_t delay_us) {
    // Enable the interrupt for our alarm (the timer outputs 4 alarm irqs)
    hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM);
    // Set irq handler for alarm irq
    irq_set_exclusive_handler(ALARM_IRQ, alarm_irq);
    // Enable the alarm irq
    irq_set_enabled(ALARM_IRQ, true);
    // Enable interrupt in block and at processor

    // Alarm is only 32 bits so if trying to delay more
    // than that need to be careful and keep track of the upper
    // bits
    uint64_t target = timer_hw->timerawl + delay_us;

    // Write the lower 32 bits of the target time to the alarm which
    // will arm it
    timer_hw->alarm[ALARM_NUM] = (uint32_t) target;
}

int main()
{
    stdio_init_all();
    sleep_ms(2500);
    printf("Hello Timer!\n");
    set_sys_clock_khz(133000, true); // 325us
                                     // GPIO initialisation.
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, 1);
    gpio_pull_up(BUILTIN_LED);

    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(3000, alarm_callback, NULL, false);//只调用一次，3s后调用
    while (!timer_fired)
    {
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        // gpio_put(BUILTIN_LED, 1);
        // sleep_ms(500);
        // gpio_put(BUILTIN_LED, 0);
        sleep_ms(500);
    }
    add_alarm_in_us(1000000 * 2, alarm_callback, NULL, false);//只调用一次，2s后调用
    while (!timer_fired)
    {
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED
        // gpio_put(BUILTIN_LED, 1);
        // sleep_ms(500);
        // gpio_put(BUILTIN_LED, 0);
        sleep_ms(500);
    }
    // Create a repeating timer that calls repeating_timer_callback.
//如果延迟 > 0，那么这是上一次回调结束和下一次开始之间的延迟。
//如果延迟是负的(见下)，那么下一个回调调用将恰好在500毫秒之后
//开始调用最后一个回调函数
    struct repeating_timer timer;
    add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);
    sleep_ms(3000);
    bool cancelled = cancel_repeating_timer(&timer);
    printf("cancelled... %d\n", cancelled);
    sleep_ms(2000);

//负延迟so意味着我们将调用repeating_timer_callback，并再次调用它
//不管回调执行了多长时间，都将在500毫秒后执行
    add_repeating_timer_ms(-500, repeating_timer_callback, NULL, &timer);
   sleep_ms(3000);
   cancelled = cancel_repeating_timer(&timer);
   printf("ms cancelled... %d\n", cancelled);
   add_repeating_timer_us(1000000 * 2, repeating_timer_callback, NULL, &timer);
   sleep_ms(3000);
   cancelled = cancel_repeating_timer(&timer);
   printf("us cancelled... %d\n", cancelled);
   sleep_ms(2000);
   printf("Done\n");
   printf("Timer lowlevel!\n");
    alarm_fired = false;
        alarm_in_us(1000000 * 8);
        //while (!alarm_fired){ tight_loop_contents();}
    while (1)
    {
        // tight_loop_contents();
        uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
        uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
        uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);
        printf("clk_sys  = %dkHz\n", f_clk_sys);
        printf("clk_usb  = %dkHz\n", f_pll_usb);
        printf("clk_rtc  = %dkHz\n", f_clk_rtc);
        sleep_ms(1000);
        gpio_xor_mask(1ul << BUILTIN_LED); // Toggle the LED

        // Wait for alarm to fire
        if (!alarm_fired){

            printf("Alarm did not fire\n");
        }
    }

    return 0;
}
