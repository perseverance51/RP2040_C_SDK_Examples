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
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Pio-UART-RX-RP2040 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Pio-UART-RX-RP2040 Pio-UART-RX-RP2040.c uart_rx.c)

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/uart_rx.pio)

pico_set_program_name(Pio-UART-RX-RP2040 "Pio-UART-RX-RP2040")
pico_set_program_version(Pio-UART-RX-RP2040 "0.1")

# Generate PIO header
pico_generate_pio_header(Pio-UART-RX-RP2040 ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Pio-UART-RX-RP2040 1)
pico_enable_stdio_usb(Pio-UART-RX-RP2040 0)

# Add the standard library to the build
target_link_libraries(Pio-UART-RX-RP2040
        pico_stdlib
        hardware_irq
        hardware_pio
        hardware_dma)

# Add the standard include files to the build
target_include_directories(Pio-UART-RX-RP2040 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Pio-UART-RX-RP2040
        hardware_pio
        )

pico_add_extra_outputs(Pio-UART-RX-RP2040)

