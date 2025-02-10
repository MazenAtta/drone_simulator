cmake_minimum_required(VERSION 3.10)
project(DroneSimulator)

set(CMAKE_CXX_STANDARD 11)

# Find Fast DDS package
# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr 2 REQUIRED)
endif()

if(NOT fastdds_FOUND)
    find_package(fastdds 3 REQUIRED)
endif()

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

# Find Curses package
find_package(Curses REQUIRED)

# Find YAML package
find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML REQUIRED yaml-0.1)

# Include directories
include_directories(
    ${fastrtps_INCLUDE_DIRS}
    ${fastcdr_INCLUDE_DIRS}
    include
    Generated
)


# Add executables
add_executable(blackboard_process src/blackboard_process.cpp)
add_executable(server_process src/server_process.cpp)
add_executable(output_window_process src/output_window_process.cpp)
add_executable(input_window_process src/input_window_process.cpp)
add_executable(obstacle_process src/obstacle_process.cpp)
add_executable(target_process src/target_process.cpp)
add_executable(watchdog_process src/watchdog_process.cpp)

# Add libraries
add_library(blackboard_handler src/blackboard_handler.cpp)
add_library(server_handler src/server_handler.cpp)
add_library(output_window_handler src/output_window_handler.cpp)
add_library(input_window_handler src/input_window_handler.cpp)
add_library(obstacle_handler src/obstacle_handler.cpp)
add_library(target_handler src/target_handler.cpp)
add_library(watchdog_handler src/watchdog_handler.cpp)

# Link libraries to executables
target_link_libraries(blackboard_process PRIVATE blackboard_handler)
target_link_libraries(server_process PRIVATE server_handler ${fastrtps_LIBRARIES} ${fastcdr_LIBRARIES} fastdds fastcdr)
target_link_libraries(output_window_process PRIVATE output_window_handler ncursesw tinfo ${YAML_LIBRARIES} m yaml)
target_link_libraries(input_window_process PRIVATE input_window_handler ncursesw tinfo)
target_link_libraries(obstacle_process PRIVATE obstacle_handler ${fastrtps_LIBRARIES} ${fastcdr_LIBRARIES} fastdds fastcdr)
target_link_libraries(target_process PRIVATE target_handler ${fastrtps_LIBRARIES} ${fastcdr_LIBRARIES} fastdds fastcdr)
target_link_libraries(watchdog_process PRIVATE watchdog_handler ${fastrtps_LIBRARIES} ${fastcdr_LIBRARIES} fastdds fastcdr)

# Link the output_window_handler library against the yaml library
target_link_libraries(output_window_handler PRIVATE ${YAML_LIBRARIES} m yaml)

# Link the input_window_handler library against the Curses library
target_link_libraries(input_window_handler PRIVATE ncursesw)

# Specify the source files for each library
target_sources(blackboard_handler PRIVATE src/blackboard_handler.cpp)
target_sources(server_handler PRIVATE src/server_handler.cpp)
target_sources(output_window_handler PRIVATE src/output_window_handler.cpp)
target_sources(input_window_handler PRIVATE src/input_window_handler.cpp)
target_sources(obstacle_handler PRIVATE src/obstacle_handler.cpp)
target_sources(target_handler PRIVATE src/target_handler.cpp)
target_sources(watchdog_handler PRIVATE src/watchdog_handler.cpp)

file(GLOB DDS_SOURCES_CXX 
    "DDS/src/Generated/*.cxx"
)

# Add generated sources for Targets and Obstacles PubSubTypes
add_library(TargetsPubSubTypes ${DDS_SOURCES_CXX})
add_library(ObstaclesPubSubTypes ${DDS_SOURCES_CXX})



# Link generated PubSubTypes to the appropriate executables
target_link_libraries(obstacle_process PRIVATE TargetsPubSubTypes ObstaclesPubSubTypes)
target_link_libraries(target_process PRIVATE TargetsPubSubTypes ObstaclesPubSubTypes)
target_link_libraries(server_process PRIVATE TargetsPubSubTypes ObstaclesPubSubTypes)
target_link_libraries(watchdog_process PRIVATE TargetsPubSubTypes ObstaclesPubSubTypes)

# Include directories for generated PubSubTypes
target_include_directories(TargetsPubSubTypes PUBLIC DDS/src/Generated)
target_include_directories(ObstaclesPubSubTypes PUBLIC DDS/src/Generated)