#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "FreeRTOS.h"
#include "task.h"

#define PLL_SYS_KHZ (133 * 1000)

struct led_task_arg {
    int gpio;
    int delay;
};

void led_task(void *p)
{
    struct led_task_arg *a = (struct led_task_arg *)p;

    gpio_init(a->gpio);
    gpio_set_dir(a->gpio, GPIO_OUT);
    while (true) {
        gpio_put(a->gpio, 1);
        vTaskDelay(pdMS_TO_TICKS(a->delay));
        gpio_put(a->gpio, 0);
        vTaskDelay(pdMS_TO_TICKS(a->delay));
    }
}

void hello_task(void *p)
{
    while (true) {
        printf("Hello, FreeRTOS! System running at %d Hz\n", clock_get_hz(clk_sys));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}




int main()
{
    // set sysclock
    set_sys_clock_khz(PLL_SYS_KHZ, true);
    stdio_init_all();

    // Timer example code - This example fires off the callback after 2000ms
  //  add_alarm_in_ms(2000, alarm_callback, NULL, false);
    // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
  //  printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));
    // For more examples of clocks use see https://github.com/raspberrypi/pico-examples/tree/master/clocks
    printf("Start LED blink\n");

    struct led_task_arg arg1 = { 16, 500 };
    xTaskCreate(led_task, "LED_Task 1", 256, &arg1, 1, NULL);

    struct led_task_arg arg2 = { 17, 800 };
    xTaskCreate(led_task, "LED_Task 2", 256, &arg2, 1, NULL);

    xTaskCreate(hello_task, "HELLO", 2048, NULL, 1, NULL);
    vTaskStartScheduler();

    while (true) {
        // printf("Hello, world!\n");
        // sleep_ms(1000);
    }
}
