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
include CMakeFiles/obstacle_handler.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/obstacle_handler.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/obstacle_handler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/obstacle_handler.dir/flags.make

CMakeFiles/obstacle_handler.dir/codegen:
.PHONY : CMakeFiles/obstacle_handler.dir/codegen

CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.o: CMakeFiles/obstacle_handler.dir/flags.make
CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.o: /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/obstacle_handler.c
CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.o: CMakeFiles/obstacle_handler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mazenatta/Desktop/Master/ARP/drone_simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.o -MF CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.o.d -o CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.o -c /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/obstacle_handler.c

CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/obstacle_handler.c > CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.i

CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/obstacle_handler.c -o CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.s

# Object files for target obstacle_handler
obstacle_handler_OBJECTS = \
"CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.o"

# External object files for target obstacle_handler
obstacle_handler_EXTERNAL_OBJECTS =

libobstacle_handler.a: CMakeFiles/obstacle_handler.dir/src/obstacle_handler.c.o
libobstacle_handler.a: CMakeFiles/obstacle_handler.dir/build.make
libobstacle_handler.a: CMakeFiles/obstacle_handler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/mazenatta/Desktop/Master/ARP/drone_simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libobstacle_handler.a"
	$(CMAKE_COMMAND) -P CMakeFiles/obstacle_handler.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/obstacle_handler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/obstacle_handler.dir/build: libobstacle_handler.a
.PHONY : CMakeFiles/obstacle_handler.dir/build

CMakeFiles/obstacle_handler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/obstacle_handler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/obstacle_handler.dir/clean

CMakeFiles/obstacle_handler.dir/depend:
	cd /home/mazenatta/Desktop/Master/ARP/drone_simulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mazenatta/Desktop/Master/ARP/drone_simulator /home/mazenatta/Desktop/Master/ARP/drone_simulator /home/mazenatta/Desktop/Master/ARP/drone_simulator/build /home/mazenatta/Desktop/Master/ARP/drone_simulator/build /home/mazenatta/Desktop/Master/ARP/drone_simulator/build/CMakeFiles/obstacle_handler.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/obstacle_handler.dir/depend

