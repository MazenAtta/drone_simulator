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
include CMakeFiles/server_handler.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/server_handler.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/server_handler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server_handler.dir/flags.make

CMakeFiles/server_handler.dir/codegen:
.PHONY : CMakeFiles/server_handler.dir/codegen

CMakeFiles/server_handler.dir/src/server_handler.cpp.o: CMakeFiles/server_handler.dir/flags.make
CMakeFiles/server_handler.dir/src/server_handler.cpp.o: /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/server_handler.cpp
CMakeFiles/server_handler.dir/src/server_handler.cpp.o: CMakeFiles/server_handler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mazenatta/Desktop/Master/ARP/drone_simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server_handler.dir/src/server_handler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_handler.dir/src/server_handler.cpp.o -MF CMakeFiles/server_handler.dir/src/server_handler.cpp.o.d -o CMakeFiles/server_handler.dir/src/server_handler.cpp.o -c /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/server_handler.cpp

CMakeFiles/server_handler.dir/src/server_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server_handler.dir/src/server_handler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/server_handler.cpp > CMakeFiles/server_handler.dir/src/server_handler.cpp.i

CMakeFiles/server_handler.dir/src/server_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server_handler.dir/src/server_handler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mazenatta/Desktop/Master/ARP/drone_simulator/src/server_handler.cpp -o CMakeFiles/server_handler.dir/src/server_handler.cpp.s

# Object files for target server_handler
server_handler_OBJECTS = \
"CMakeFiles/server_handler.dir/src/server_handler.cpp.o"

# External object files for target server_handler
server_handler_EXTERNAL_OBJECTS =

libserver_handler.a: CMakeFiles/server_handler.dir/src/server_handler.cpp.o
libserver_handler.a: CMakeFiles/server_handler.dir/build.make
libserver_handler.a: CMakeFiles/server_handler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/mazenatta/Desktop/Master/ARP/drone_simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libserver_handler.a"
	$(CMAKE_COMMAND) -P CMakeFiles/server_handler.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server_handler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server_handler.dir/build: libserver_handler.a
.PHONY : CMakeFiles/server_handler.dir/build

CMakeFiles/server_handler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server_handler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server_handler.dir/clean

CMakeFiles/server_handler.dir/depend:
	cd /home/mazenatta/Desktop/Master/ARP/drone_simulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mazenatta/Desktop/Master/ARP/drone_simulator /home/mazenatta/Desktop/Master/ARP/drone_simulator /home/mazenatta/Desktop/Master/ARP/drone_simulator/build /home/mazenatta/Desktop/Master/ARP/drone_simulator/build /home/mazenatta/Desktop/Master/ARP/drone_simulator/build/CMakeFiles/server_handler.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/server_handler.dir/depend

