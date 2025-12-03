# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")
set(FREERTOS_SRC_DIRECTORY "${CMAKE_SOURCE_DIR}/FreeRTOS/FreeRTOS-Kernel")
# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
# include(FreeRTOS_Kernel_Import.cmake)

project(blink_freertos C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(blink_freertos blink_freertos.c )

pico_set_program_name(blink_freertos "blink_freertos")
pico_set_program_version(blink_freertos "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(blink_freertos 1)
pico_enable_stdio_usb(blink_freertos 0)

# set(PICO_HEAP_SIZE 4096)

# FreeRTOS configuration for RP2040
#  set(FREERTOS_PORT GCC_ARM_CM0 CACHE STRING "FreeRTOS port for RP2040 (ARM Cortex-M0+)")

# Create freertos_config target as required by FreeRTOS-Kernel

# set(FREERTOS_KERNEL_PATH ${CMAKE_SOURCE_DIR}/FreeRTOS)

# Add FreeRTOS as a library
add_library(FreeRTOS STATIC
    ${FREERTOS_SRC_DIRECTORY}/event_groups.c
    ${FREERTOS_SRC_DIRECTORY}/list.c
    ${FREERTOS_SRC_DIRECTORY}/queue.c
    ${FREERTOS_SRC_DIRECTORY}/stream_buffer.c
    ${FREERTOS_SRC_DIRECTORY}/tasks.c
    ${FREERTOS_SRC_DIRECTORY}/timers.c
    ${FREERTOS_SRC_DIRECTORY}/portable/MemMang/heap_3.c
    ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0/port.c
    ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0/portasm.c
)

# Build FreeRTOS
target_include_directories(FreeRTOS PUBLIC
    ${CMAKE_SOURCE_DIR}/FreeRTOS/include
    ${FREERTOS_CFG_DIRECTORY}/
    ${FREERTOS_SRC_DIRECTORY}/include
    ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0
)

#add_subdirectory(FreeRTOS)
# Add the standard library to the build
target_link_libraries(blink_freertos
        pico_stdlib
        # FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
        # pico_multicore
        FreeRTOS
        )



# Add the standard include files to the build
target_include_directories(blink_freertos PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS/include
        # ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS/FreeRTOS-Kernel/include
        # ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS/FreeRTOS-Kernel/portable/GCC/ARM_CM0
)

# Add any user requested libraries
target_link_libraries(blink_freertos
        hardware_timer
        hardware_clocks
        hardware_gpio

        )

pico_add_extra_outputs(blink_freertos)
