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
CMAKE_SOURCE_DIR = D:/RP2040/pico-project-generator/RP2040_DHT11_PIO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/build

# Utility rule file for dht_dht_pio_h.

# Include any custom commands dependencies for this target.
include dht/CMakeFiles/dht_dht_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include dht/CMakeFiles/dht_dht_pio_h.dir/progress.make

dht/CMakeFiles/dht_dht_pio_h: dht/dht.pio.h

dht/dht.pio.h: D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/dht/dht.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating dht.pio.h"
	cd D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/build/dht && "F:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk-tools/pioasm.exe" -o c-sdk D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/dht/dht.pio D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/build/dht/dht.pio.h

dht_dht_pio_h: dht/CMakeFiles/dht_dht_pio_h
dht_dht_pio_h: dht/dht.pio.h
dht_dht_pio_h: dht/CMakeFiles/dht_dht_pio_h.dir/build.make
.PHONY : dht_dht_pio_h

# Rule to build all files generated by this target.
dht/CMakeFiles/dht_dht_pio_h.dir/build: dht_dht_pio_h
.PHONY : dht/CMakeFiles/dht_dht_pio_h.dir/build

dht/CMakeFiles/dht_dht_pio_h.dir/clean:
	cd D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/build/dht && $(CMAKE_COMMAND) -P CMakeFiles/dht_dht_pio_h.dir/cmake_clean.cmake
.PHONY : dht/CMakeFiles/dht_dht_pio_h.dir/clean

dht/CMakeFiles/dht_dht_pio_h.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" D:/RP2040/pico-project-generator/RP2040_DHT11_PIO D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/dht D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/build D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/build/dht D:/RP2040/pico-project-generator/RP2040_DHT11_PIO/build/dht/CMakeFiles/dht_dht_pio_h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dht/CMakeFiles/dht_dht_pio_h.dir/depend

