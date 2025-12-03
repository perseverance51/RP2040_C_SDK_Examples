#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "FreeRTOS.h"
#include "task.h"

#define PLL_SYS_KHZ (133 * 1000)

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

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
        printf("Hello, FreeRTOS!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask1_Code( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    */
    for( ;; )
    {
             gpio_put(25, 1);
            vTaskDelay(1000);
            gpio_put(25, 0);
            vTaskDelay(1000);
    }
}

void vTaskCode( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    */
    for( ;; )
    {
             gpio_put(25, 1);
            vTaskDelay(1000);
            gpio_put(25, 0);
            vTaskDelay(1000);
    }
}

int main()
{
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    // 设置系统时钟为133MHz
  //  set_sys_clock_khz(PLL_SYS_KHZ, true);
    stdio_init_all();

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    printf("FreeRTOS starting...\n");

    // 创建FreeRTOS任务并检查返回值
    // xReturned = xTaskCreate(hello_task, "HELLO", 2048, NULL, 2, &xHandle);

    // if (xReturned != pdPASS) {
    //     printf("ERROR: Failed to create hello_task (error code: %ld)\n", xReturned);
    //     while (1) {
    //         // 如果任务创建失败，快速闪烁LED
    //         gpio_put(LED_PIN, 1);
    //         sleep_ms(250);
    //         gpio_put(LED_PIN, 0);
    //         sleep_ms(250);
    //     }
    // }
    xReturned = xTaskCreate(
                    vTask1_Code,       /* Function that implements the task. */
                    "Blinky task",   /* Text name for the task. */
                    512,             /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle );

    vTaskStartScheduler();

    printf("Task created successfully, starting scheduler...\n");

    // 启动FreeRTOS调度器
  //  vTaskStartScheduler();

    // 如果调度器启动失败，应该不会到达这里
    printf("ERROR: FreeRTOS scheduler failed to start!\n");

    // 调度器失败时的处理
    while (1) {
         configASSERT(0);    /* We should never get here */
        // 快速闪烁LED表示错误
        // gpio_put(LED_PIN, 1);
        // sleep_ms(500);
        // gpio_put(LED_PIN, 0);
        // sleep_ms(500);
    }
}
