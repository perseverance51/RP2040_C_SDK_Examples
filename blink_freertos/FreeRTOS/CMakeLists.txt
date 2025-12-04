# FreeRTOS CMake configuration for RP2040
add_library(FreeRTOS STATIC
    FreeRTOS-Kernel/croutine.c
    FreeRTOS-Kernel/event_groups.c
    FreeRTOS-Kernel/list.c
    FreeRTOS-Kernel/queue.c
    FreeRTOS-Kernel/stream_buffer.c
    FreeRTOS-Kernel/tasks.c
    FreeRTOS-Kernel/timers.c
    # Use ARM_CM0 port layer (RP2040 uses Cortex-M0+)
    FreeRTOS-Kernel/portable/GCC/ARM_CM0/port.c
    FreeRTOS-Kernel/portable/MemMang/heap_4.c
)

# Set include directories for FreeRTOS
target_include_directories(FreeRTOS PUBLIC
    include
    FreeRTOS-Kernel/include
    FreeRTOS-Kernel/portable/GCC/ARM_CM0
)

# Set compiler flags for FreeRTOS
target_compile_options(FreeRTOS PRIVATE
    -Wno-unused-parameter
    -Wno-sign-compare
)
