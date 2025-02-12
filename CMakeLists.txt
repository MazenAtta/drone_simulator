cmake_minimum_required(VERSION 3.10)
project(DroneSimulator)

set(CMAKE_CXX_STANDARD 11)

# Find Fast DDS package
find_package(fastcdr 2 REQUIRED)
find_package(fastdds 3 REQUIRED)

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
    CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(SUPPORTS_CXX11)
    add_compile_options(-std=c++11)
    else()
    message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

# Find Curses and YAML packages
find_package(Curses REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML REQUIRED yaml-0.1)

# Include directories
include_directories(
    ${fastrtps_INCLUDE_DIRS}
    ${fastcdr_INCLUDE_DIRS}
    include
    Generated
    /usr/local/include
)

# Add executables
set(EXECUTABLES
    blackboard_process
    server_process
    output_window_process
    input_window_process
    obstacle_process
    target_process
    watchdog_process
)

foreach(EXE ${EXECUTABLES})
    add_executable(${EXE} src/${EXE}.cpp)
endforeach()

# Add libraries
set(LIBRARIES
    blackboard_handler
    server_handler
    output_window_handler
    input_window_handler
    obstacle_handler
    target_handler
    watchdog_handler
)

foreach(LIB ${LIBRARIES})
    add_library(${LIB} src/${LIB}.cpp)
endforeach()

# Link libraries to executables
target_link_libraries(blackboard_process PRIVATE blackboard_handler)
target_link_libraries(output_window_process PRIVATE output_window_handler ncursesw tinfo ${YAML_LIBRARIES} m yaml)
target_link_libraries(input_window_process PRIVATE input_window_handler ncursesw tinfo)
target_link_libraries(server_process PRIVATE server_handler fastcdr fastdds ${fastrtps_LIBRARIES} ${fastcdr_LIBRARIES})
target_link_libraries(obstacle_process PRIVATE obstacle_handler fastcdr fastdds ${fastrtps_LIBRARIES} ${fastcdr_LIBRARIES})
target_link_libraries(target_process PRIVATE target_handler fastcdr fastdds ${fastrtps_LIBRARIES} ${fastcdr_LIBRARIES})
target_link_libraries(watchdog_process PRIVATE watchdog_handler fastcdr fastdds ${fastrtps_LIBRARIES} ${fastcdr_LIBRARIES})

# Link the output_window_handler library against the yaml library
target_link_libraries(output_window_handler PRIVATE ${YAML_LIBRARIES} m yaml)

# Link the input_window_handler library against the Curses library
target_link_libraries(input_window_handler PRIVATE ncursesw)

# Add generated sources for Targets and Obstacles PubSubTypes
file(GLOB DDS_SOURCES_CXX "DDS/src/Generated/*.cxx")
add_library(TargetsPubSubTypes ${DDS_SOURCES_CXX})
add_library(ObstaclesPubSubTypes ${DDS_SOURCES_CXX})

# Link generated PubSubTypes to the appropriate executables
foreach(EXE IN ITEMS obstacle_process target_process server_process watchdog_process)
    target_link_libraries(${EXE} PRIVATE TargetsPubSubTypes ObstaclesPubSubTypes)
endforeach()

# Include directories for generated PubSubTypes
target_include_directories(TargetsPubSubTypes PUBLIC DDS/src/Generated)
target_include_directories(ObstaclesPubSubTypes PUBLIC DDS/src/Generated)
