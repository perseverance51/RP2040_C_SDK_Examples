# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "G:/Raspberry Pi/Pico SDK v1.5.1/pico-sdk")

set(PICO_BOARD pimoroni_picolipo_16mb CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(WouoUIPage C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(WouoUIPage
WouoUIPage.c
${CMAKE_CURRENT_LIST_DIR}/OLED/oled.c
${CMAKE_CURRENT_LIST_DIR}/OLED/oled_ui.c
${CMAKE_CURRENT_LIST_DIR}/OLED/oled_g.c
${CMAKE_CURRENT_LIST_DIR}/TestUI/TestUI.c)

pico_set_program_name(WouoUIPage "WouoUIPage")
pico_set_program_version(WouoUIPage "0.1")

pico_enable_stdio_uart(WouoUIPage 1)
pico_enable_stdio_usb(WouoUIPage 0)

# Add the standard library to the build
target_link_libraries(WouoUIPage
        pico_stdlib)

# Add the standard include files to the build 锟斤拷锟斤拷头锟侥硷拷目录
target_include_directories(WouoUIPage PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  ${CMAKE_CURRENT_LIST_DIR}/OLED
  ${CMAKE_CURRENT_LIST_DIR}/TestUI
)

# Add any user requested libraries 锟斤拷锟斤拷 Pico 锟斤拷
target_link_libraries(WouoUIPage
        hardware_i2c
        )
# 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟侥硷拷
pico_add_extra_outputs(WouoUIPage)

