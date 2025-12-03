add_library(FreeRTOS STATIC FreeRTOS-Kernel/croutine.c
    FreeRTOS-Kernel/event_groups.c
    FreeRTOS-Kernel/list.c
    FreeRTOS-Kernel/queue.c
    FreeRTOS-Kernel/stream_buffer.c
    FreeRTOS-Kernel/tasks.c
    FreeRTOS-Kernel/timers.c
     FreeRTOS-Kernel/portable/GCC/ARM_CM0/port.c
     FreeRTOS-Kernel/portable/GCC/ARM_CM0/portasm.c
    FreeRTOS-Kernel/portable/MemMang/heap_4.c
    src/FreeRTOS_Support_Files/IdleMemory.c
    # port.c
)

target_include_directories(FreeRTOS PUBLIC include FreeRTOS-Kernel/include FreeRTOS-Kernel/portable/GCC/ARM_CM0)
