# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "F:/Raspberry Pi/Pico SDK v1.5.1/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(RP2040_DHT11_PIO C CXX ASM)

#set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(dht)
# Add executable. Default name is the project name, version 0.1

add_executable(RP2040_DHT11_PIO  RP2040_DHT11_PIO.c
        dht/dht.c)

# Add any user requested libraries
pico_generate_pio_header(RP2040_DHT11_PIO ${CMAKE_CURRENT_LIST_DIR}/dht/dht.pio)



pico_set_program_name(RP2040_DHT11_PIO "RP2040_DHT11_PIO")
pico_set_program_version(RP2040_DHT11_PIO "0.1")

pico_enable_stdio_uart(RP2040_DHT11_PIO 1)
pico_enable_stdio_usb(RP2040_DHT11_PIO 0)

# Add the standard library to the build
target_link_libraries(RP2040_DHT11_PIO
        pico_stdlib
        hardware_adc
        hardware_pio
        hardware_gpio
        hardware_dma)

# Add the standard include files to the build
target_include_directories(RP2040_DHT11_PIO PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
    ${CMAKE_CURRENT_LIST_DIR}/dht/include
)


pico_add_extra_outputs(RP2040_DHT11_PIO)

