# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "G:/Raspberry Pi/Pico SDK v1.5.1/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(RP2040_ST7735tft_DISPLAY C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# library configuration (override if necessary, see pico-st7735 for a complete list)
set(SPI_TFT_PORT "spi0" CACHE STRING "TFT spi port-name")
set(SPI_TFT_CS   "9"    CACHE STRING "TFT CS  pin number")  #9引脚
set(SPI_TFT_DC   "10"   CACHE STRING "TFT DC  pin number")  #10引脚
set(SPI_TFT_RST  "11"   CACHE STRING "TFT RST pin number")  #11引脚
set(TFT_OPTIONS TFT_ENABLE_BLACK TFT_ENABLE_RESET TFT_ENABLE_TEXT TFT_ENABLE_SHAPES
                TFT_ENABLE_ROTATE TFT_ENABLE_SCROLL
            CACHE STRING "TFT options/functions")
#program configuration
set(SPI_PORT "spi0" CACHE STRING "spi port-name")
set(SPI_RX   "8"   CACHE STRING "RX  pin number")
set(SPI_TX   "7"   CACHE STRING "TX  pin number")  #SDA引脚 15引脚
set(SPI_SCK  "6"   CACHE STRING "SCK pin number")  #SCL引脚 14引脚

add_compile_definitions(SPI_PORT=${SPI_PORT})
add_compile_definitions(SPI_RX=${SPI_RX})
add_compile_definitions(SPI_TX=${SPI_TX})
add_compile_definitions(SPI_SCK=${SPI_SCK})
foreach(opt IN LISTS TFT_OPTIONS)
    message(">>> using: ${opt}")
    add_compile_definitions(${opt})
endforeach(opt)

# ENABLE_FONTS: set on command-line or select from
# VSCode-menu (see .vscode/cmake-variants.json)
if(ENABLE_FONTS)
  message(">>> Font test")
  add_compile_definitions(TFT_ENABLE_FONTS ENABLE_TEST9 ENABLE_TEST9A)
else()
  message(">>> Normal test")
  add_compile_definitions(ENABLE_TESTR ENABLE_TEST1 ENABLE_TEST1A
                           ENABLE_TEST2 ENABLE_TEST3 ENABLE_TEST4
                           ENABLE_TEST5 ENABLE_TEST6 ENABLE_TEST7 ENABLE_TEST10)
endif()
# add preprocessor-constant DEBUG for Debug-builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_VERBOSE_MAKEFILE 1)
  add_compile_definitions(DEBUG)
else()
endif()
# Add executable. Default name is the project name, version 0.1

add_executable(RP2040_ST7735tft_DISPLAY RP2040_ST7735tft_DISPLAY.c
tst_funcs.c)

pico_set_program_name(RP2040_ST7735tft_DISPLAY "RP2040_ST7735tft_DISPLAY")
pico_set_program_version(RP2040_ST7735tft_DISPLAY "0.1")

pico_enable_stdio_uart(RP2040_ST7735tft_DISPLAY 1)
pico_enable_stdio_usb(RP2040_ST7735tft_DISPLAY 0)

# Add the standard library to the build
target_link_libraries(RP2040_ST7735tft_DISPLAY
        pico_stdlib)
# build the st7735-library from our submodule
add_subdirectory(pico-st7735)
# Add the standard include files to the build
target_include_directories(RP2040_ST7735tft_DISPLAY PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(RP2040_ST7735tft_DISPLAY
        hardware_spi
        st7735
        )

pico_add_extra_outputs(RP2040_ST7735tft_DISPLAY)

