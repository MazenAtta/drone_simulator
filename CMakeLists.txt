cmake_minimum_required(VERSION 3.10)
project(DroneSimulator)

# add_executable(example example.c)
add_executable(blackboard_process src/blackboard_process.c)
add_executable(server_process src/server_process.c)
add_executable(output_window_process src/output_window_process.c)
add_executable(input_window_process src/input_window_process.c)
add_executable(obstacle_process src/obstacle_process.c)
add_executable(target_process src/target_process.c)
add_executable(watchdog_process src/watchdog_process.c)

add_library(blackboard_handler src/blackboard_handler.c)
add_library(server_handler src/server_handler.c)
add_library(output_window_handler src/output_window_handler.c)
add_library(input_window_handler src/input_window_handler.c)
add_library(obstacle_handler src/obstacle_handler.c)
add_library(target_handler src/target_handler.c)
add_library(watchdog_handler src/watchdog_handler.c)

find_package(Curses REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML REQUIRED yaml-0.1)

target_include_directories(blackboard_handler PUBLIC include)
target_include_directories(server_handler PUBLIC include)
target_include_directories(output_window_handler PUBLIC include)
target_include_directories(input_window_handler PUBLIC include)
target_include_directories(obstacle_handler PUBLIC include)
target_include_directories(target_handler PUBLIC include)
target_include_directories(watchdog_handler PUBLIC include)

target_link_libraries(blackboard_process PRIVATE blackboard_handler)
target_link_libraries(server_process PRIVATE server_handler)
target_link_libraries(output_window_process PRIVATE ncursesw ${YAML_LIBRARIES} m yaml)
target_link_libraries(output_window_process PRIVATE output_window_handler)
target_link_libraries(input_window_process PRIVATE ncursesw)
target_link_libraries(input_window_process PRIVATE input_window_handler)
target_link_libraries(obstacle_process PRIVATE obstacle_handler)
target_link_libraries(target_process PRIVATE target_handler)
target_link_libraries(watchdog_process PRIVATE watchdog_handler)

# Link the output_window_handler library against the yaml library
target_link_libraries(output_window_handler PRIVATE ${YAML_LIBRARIES} m yaml)

# Link the input_window_handler library against the Curses library
target_link_libraries(input_window_handler PRIVATE ncursesw)
target_link_libraries(input_window_handler PRIVATE ncursesw)