# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mazenatta/Desktop/Master/ARP/drone_simulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mazenatta/Desktop/Master/ARP/drone_simulator/build

# Include any dependencies generated for this target.
include CMakeFiles/output_process.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/output_process.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/output_process.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/output_process.dir/flags.make

CMakeFiles/output_process.dir/codegen:
.PHONY : CMakeFiles/output_process.dir/codegen

CMakeFiles/output_process.dir/src/output_window_process.c.o: CMakeFiles/output_process.dir/flags.make
CMakeFiles/output_process.dir/src/output_window_process.c.o: /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/output_window_process.c
CMakeFiles/output_process.dir/src/output_window_process.c.o: CMakeFiles/output_process.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mazenatta/Desktop/Master/ARP/drone_simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/output_process.dir/src/output_window_process.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/output_process.dir/src/output_window_process.c.o -MF CMakeFiles/output_process.dir/src/output_window_process.c.o.d -o CMakeFiles/output_process.dir/src/output_window_process.c.o -c /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/output_window_process.c

CMakeFiles/output_process.dir/src/output_window_process.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/output_process.dir/src/output_window_process.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/output_window_process.c > CMakeFiles/output_process.dir/src/output_window_process.c.i

CMakeFiles/output_process.dir/src/output_window_process.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/output_process.dir/src/output_window_process.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/output_window_process.c -o CMakeFiles/output_process.dir/src/output_window_process.c.s

# Object files for target output_process
output_process_OBJECTS = \
"CMakeFiles/output_process.dir/src/output_window_process.c.o"

# External object files for target output_process
output_process_EXTERNAL_OBJECTS =

output_process: CMakeFiles/output_process.dir/src/output_window_process.c.o
output_process: CMakeFiles/output_process.dir/build.make
output_process: /usr/lib/x86_64-linux-gnu/libcurses.so
output_process: /usr/lib/x86_64-linux-gnu/libform.so
output_process: liboutput_window_handler.a
output_process: CMakeFiles/output_process.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/mazenatta/Desktop/Master/ARP/drone_simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable output_process"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/output_process.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/output_process.dir/build: output_process
.PHONY : CMakeFiles/output_process.dir/build

CMakeFiles/output_process.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/output_process.dir/cmake_clean.cmake
.PHONY : CMakeFiles/output_process.dir/clean

CMakeFiles/output_process.dir/depend:
	cd /home/mazenatta/Desktop/Master/ARP/drone_simulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mazenatta/Desktop/Master/ARP/drone_simulator /home/mazenatta/Desktop/Master/ARP/drone_simulator /home/mazenatta/Desktop/Master/ARP/drone_simulator/build /home/mazenatta/Desktop/Master/ARP/drone_simulator/build /home/mazenatta/Desktop/Master/ARP/drone_simulator/build/CMakeFiles/output_process.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/output_process.dir/depend

