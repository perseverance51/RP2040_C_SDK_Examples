#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "FreeRTOS.h"
#include "task.h"

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

struct led_task_arg {
    int gpio;
    int delay;
};

void led1_task(void *p)
{
    struct led_task_arg *a = (struct led_task_arg *)p;

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    while (true) {
        gpio_put(25, 1);
            vTaskDelay(1000);
            gpio_put(25, 0);
            vTaskDelay(1000);
    }
}

void led2_task(void *p)
{
    struct led_task_arg *a = (struct led_task_arg *)p;

    gpio_init(16);
    gpio_set_dir(16, GPIO_OUT);
    while (true) {
        gpio_put(16, 1);
            vTaskDelay(1000);
            gpio_put(16, 0);
            vTaskDelay(1000);
    }
}

void hello_task(void *p)
{
    while (true) {
        printf("Hello, FreeRTOS!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


int main()
{

    stdio_init_all();

    // Timer example code - This example fires off the callback after 2000ms
 //   add_alarm_in_ms(2000, alarm_callback, NULL, false);
    // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

  //  printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
 //   printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));
    // For more examples of clocks use see https://github.com/raspberrypi/pico-examples/tree/master/clocks
    xTaskCreate(
                    led1_task,       /* Function that implements the task. */
                    "Blinky task",   /* Text name for the task. */
                    512,             /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    NULL );
    xTaskCreate(
                    led2_task,       /* Function that implements the task. */
                    "Blinky task",   /* Text name for the task. */
                    512,             /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    NULL );
    xTaskCreate(
                    hello_task,       /* Function that implements the task. */
                    "Hello task",   /* Text name for the task. */
                    512,             /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    NULL );

     vTaskStartScheduler();
    while (true) {
        // printf("Hello, world!\n");
        // sleep_ms(1000);
    }
}
