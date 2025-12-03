#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Use Pico SDK ISR handlers */
#define vPortSVCHandler         isr_svcall
#define xPortPendSVHandler      isr_pendsv
#define xPortSysTickHandler     isr_systick

/* Clock Configuration */
#define configCPU_CLOCK_HZ      125000000  /* RP2040系统时钟频率125MHz */
#define configSYSTICK_CLOCK_HZ  1000000    /* SysTick时钟频率1MHz */

/* Scheduler Related */
#define configUSE_PREEMPTION                    1
#define configUSE_TICKLESS_IDLE                 0
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                    5
#define configMINIMAL_STACK_SIZE                ( configSTACK_DEPTH_TYPE ) 128
#define configUSE_16_BIT_TICKS                  0

#define configIDLE_SHOULD_YIELD                 1

/* Synchronization Related */
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_COUNTING_SEMAPHORES           1
#define configQUEUE_REGISTRY_SIZE               8
#define configUSE_QUEUE_SETS                    1
#define configUSE_TIME_SLICING                  1
#define configUSE_NEWLIB_REENTRANT              0
#define configENABLE_BACKWARD_COMPATIBILITY     1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5

/* System */
#define configSTACK_DEPTH_TYPE                  uint32_t
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         0  /* 使用动态内存分配 */
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   (128*1024)  /* 128KB堆大小 */
#define configAPPLICATION_ALLOCATED_HEAP        0

/* Hook function related definitions. */
#define configCHECK_FOR_STACK_OVERFLOW          0
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            128

/* Interrupt nesting behaviour configuration. */
/* For Cortex-M0+, these should be set to 0 */
#define configKERNEL_INTERRUPT_PRIORITY         0
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    0
#define configMAX_API_CALL_INTERRUPT_PRIORITY   0

/* Set configENABLE_MPU to 1 to enable the Memory Protection Unit (MPU), or 0
 * to leave the Memory Protection Unit disabled. */
#define configENABLE_MPU                        0

/* SMP port only */
#define configNUM_CORES                         0
#define configTICK_CORE                         0
#define configRUN_MULTIPLE_PRIORITIES           0

/* RP2040 specific */
#define configSUPPORT_PICO_SYNC_INTEROP         1
#define configSUPPORT_PICO_TIME_INTEROP         1

#include <assert.h>
/* Define to trap errors during development. */
#define configASSERT(x)                         assert(x)

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xTaskGetIdleTaskHandle          1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskAbortDelay                 1
#define INCLUDE_xTaskGetHandle                  1
#define INCLUDE_xTaskResumeFromISR              1
#define INCLUDE_xTaskCreate                     1  /* 添加xTaskCreate函数包含 */

/* A header file that defines trace macro can be included here. */

#endif /* FREERTOS_CONFIG_H */
