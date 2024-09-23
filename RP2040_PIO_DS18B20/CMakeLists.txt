# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "F:/Raspberry Pi/Pico SDK v1.5.1/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

set(PICO_CXX_ENABLE_EXCEPTIONS 1) # Enable C++ exceptions

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(RP2040_PIO_DS18B20 C CXX ASM)



# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(RP2040_PIO_DS18B20
                                  RP2040_PIO_DS18B20.cpp
                                  DS18B20.h
                                  DS18B20.cpp)
# Add any user requested libraries
pico_generate_pio_header(RP2040_PIO_DS18B20 ${CMAKE_CURRENT_LIST_DIR}/DS1820.pio)

pico_set_program_name(RP2040_PIO_DS18B20 "RP2040_PIO_DS18B20")
pico_set_program_version(RP2040_PIO_DS18B20 "0.1")

pico_enable_stdio_uart(RP2040_PIO_DS18B20 1)
pico_enable_stdio_usb(RP2040_PIO_DS18B20 0)

# Add the standard library to the build
target_link_libraries(RP2040_PIO_DS18B20
        pico_stdlib
        hardware_adc
        hardware_pio
        hardware_gpio

        )

# Add the standard include files to the build
target_include_directories(RP2040_PIO_DS18B20 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(RP2040_PIO_DS18B20
        hardware_pio
        hardware_clocks
        )

pico_add_extra_outputs(RP2040_PIO_DS18B20)

