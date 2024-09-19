# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "F:/Raspberry Pi/Pico SDK v1.5.1/cmake/bin/cmake.exe"

# The command to remove a file.
RM = "F:/Raspberry Pi/Pico SDK v1.5.1/cmake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:/RP2040/pico-project-generator/RP2040_ADC_IRQ

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:/RP2040/pico-project-generator/RP2040_ADC_IRQ/build

# Utility rule file for tinyusb_pico_pio_usb_usb_rx_pio_h.

# Include any custom commands dependencies for this target.
include pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/progress.make

pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h: pico-sdk/src/rp2_common/tinyusb/usb_rx.pio.h

pico-sdk/src/rp2_common/tinyusb/usb_rx.pio.h: F:/Raspberry\ Pi/Pico\ SDK\ v1.5.1/pico-sdk/lib/tinyusb/hw/mcu/raspberry_pi/Pico-PIO-USB/src/usb_rx.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/RP2040/pico-project-generator/RP2040_ADC_IRQ/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating usb_rx.pio.h"
	cd D:/RP2040/pico-project-generator/RP2040_ADC_IRQ/build/pico-sdk/src/rp2_common/tinyusb && "F:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk-tools/pioasm.exe" -o c-sdk "F:/Raspberry Pi/Pico SDK v1.5.1/pico-sdk/lib/tinyusb/hw/mcu/raspberry_pi/Pico-PIO-USB/src/usb_rx.pio" D:/RP2040/pico-project-generator/RP2040_ADC_IRQ/build/pico-sdk/src/rp2_common/tinyusb/usb_rx.pio.h

tinyusb_pico_pio_usb_usb_rx_pio_h: pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h
tinyusb_pico_pio_usb_usb_rx_pio_h: pico-sdk/src/rp2_common/tinyusb/usb_rx.pio.h
tinyusb_pico_pio_usb_usb_rx_pio_h: pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/build.make
.PHONY : tinyusb_pico_pio_usb_usb_rx_pio_h

# Rule to build all files generated by this target.
pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/build: tinyusb_pico_pio_usb_usb_rx_pio_h
.PHONY : pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/build

pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/clean:
	cd D:/RP2040/pico-project-generator/RP2040_ADC_IRQ/build/pico-sdk/src/rp2_common/tinyusb && $(CMAKE_COMMAND) -P CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/cmake_clean.cmake
.PHONY : pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/clean

pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" D:/RP2040/pico-project-generator/RP2040_ADC_IRQ "F:/Raspberry Pi/Pico SDK v1.5.1/pico-sdk/src/rp2_common/tinyusb" D:/RP2040/pico-project-generator/RP2040_ADC_IRQ/build D:/RP2040/pico-project-generator/RP2040_ADC_IRQ/build/pico-sdk/src/rp2_common/tinyusb D:/RP2040/pico-project-generator/RP2040_ADC_IRQ/build/pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pico-sdk/src/rp2_common/tinyusb/CMakeFiles/tinyusb_pico_pio_usb_usb_rx_pio_h.dir/depend

