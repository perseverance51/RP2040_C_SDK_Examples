#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "FreeRTOS.h"
#include "task.h"

#define PLL_SYS_KHZ (125 * 1000)
//volatile QueueHandle_t queue = NULL;
// Set a delay time of exactly 500ms
const TickType_t ms_delay = 500 / portTICK_PERIOD_MS;


TaskHandle_t pico_task_handle = NULL;

void led_task(void *p)
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));  // 使用正确的延时函数
        gpio_put(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void hello_task(void *p)
{
    while (true) {
        printf("Hello, FreeRTOS! System running at %d Hz\n", clock_get_hz(clk_sys));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void led_task_pico(void* unused_arg) {

    // Store the Pico LED state
    uint8_t pico_led_state = 0;

    // Configure the Pico's on-board LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (true) {
        // Turn Pico LED on an add the LED state
        // to the FreeRTOS xQUEUE
        printf("PICO LED FLASH");
        pico_led_state = 1;
        gpio_put(PICO_DEFAULT_LED_PIN, pico_led_state);
     //   xQueueSendToBack(queue, &pico_led_state, 0);
        vTaskDelay(ms_delay);

        // Turn Pico LED off an add the LED state
        // to the FreeRTOS xQUEUE
        pico_led_state = 0;
        gpio_put(PICO_DEFAULT_LED_PIN, pico_led_state);
    //    xQueueSendToBack(queue, &pico_led_state, 0);
        vTaskDelay(ms_delay);
    }
}

int main()
{
    // BaseType_t xReturned;
    // TaskHandle_t xHandle = NULL;

    // 设置系统时钟
    set_sys_clock_khz(PLL_SYS_KHZ, true);

    // 初始化标准IO
    stdio_init_all();

    // 等待串口初始化完成
    sleep_ms(1000);

    printf("=== FreeRTOS RP2040 Demo Started ===\n");
    printf("System Clock Frequency: %d Hz\n", clock_get_hz(clk_sys));

    // 创建LED闪烁任务
  //  xReturned = xTaskCreate(led_task, "LED", 1024, NULL, 1, &xHandle);

    // if (xReturned != pdPASS) {
    //     printf("ERROR: Failed to create LED task\n");
    //     while (1) {
    //         // 快速闪烁表示错误
    //         gpio_put(PICO_DEFAULT_LED_PIN, 1);
    //         sleep_ms(100);
    //         gpio_put(PICO_DEFAULT_LED_PIN, 0);
    //         sleep_ms(100);
    //     }
    // }
    BaseType_t pico_status = xTaskCreate(led_task_pico,
                                         "PICO_LED_TASK",
                                         128,
                                         NULL,
                                         1,
                                         &pico_task_handle);
    // 创建Hello任务
 //   xReturned = xTaskCreate(hello_task, "HELLO", 2048, NULL, 1, NULL);

    // if (xReturned != pdPASS) {
    //     printf("ERROR: Failed to create Hello task\n");
    //     while (1) {
    //         // 快速闪烁表示错误
    //         gpio_put(PICO_DEFAULT_LED_PIN, 1);
    //         sleep_ms(200);
    //         gpio_put(PICO_DEFAULT_LED_PIN, 0);
    //         sleep_ms(200);
    //     }
    // }

    printf("Tasks created successfully, starting scheduler...\n");

    // 启动FreeRTOS调度器（只调用一次）
    if (pico_status == pdPASS) {
        vTaskStartScheduler();
    }

    // 如果调度器启动失败，应该不会到达这里
    printf("ERROR: FreeRTOS scheduler failed to start!\n");

    // 调度器失败时的处理
    while (1) {
      //  configASSERT(0);  // 触发断言
    }
}
