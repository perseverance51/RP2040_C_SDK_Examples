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

project(FreeRTOS_Pico C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
add_subdirectory(FreeRTOS-Kernel)         #将FreeRTOS作为子模块添加到工程
include_directories(
FreeRTOS-Kernel/include                    #增加freertos的include目录
)
# Add executable. Default name is the project name, version 0.1

add_executable(FreeRTOS_Pico FreeRTOS_Pico.c )

pico_set_program_name(FreeRTOS_Pico "FreeRTOS_Pico")
pico_set_program_version(FreeRTOS_Pico "0.1")

pico_enable_stdio_uart(FreeRTOS_Pico 1)
pico_enable_stdio_usb(FreeRTOS_Pico 0)

# Add the standard library to the build
target_link_libraries(FreeRTOS_Pico
        pico_stdlib
        freertos_kernel                  #静态链接Freertos库
        hardware_gpio                    #GPIO用来测试freertos
        )

# Add the standard include files to the build
target_include_directories(FreeRTOS_Pico PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)
# include_directories($ENV{PICO_SDK_PATH}/src/common/pico_base/include)
# include_directories($ENV{PICO_SDK_PATH}/src/rp2_common/hardware_sync/include)
# Add any user requested libraries
# target_link_libraries(FreeRTOS_Pico )

pico_add_extra_outputs(FreeRTOS_Pico)

