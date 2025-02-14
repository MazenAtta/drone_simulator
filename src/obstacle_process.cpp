#include "obstacle_handler.hpp"
#include "../DDS/src/ObstaclesPublisher.hpp"


void convert_to_dds_obstacles(const Obstacle& src, Obstacles& dest)
{
    dest.obstacles_x().clear();
    dest.obstacles_y().clear();
    for (int i = 0; i < MAX_OBSTACLES; ++i)
    {
        dest.obstacles_x().push_back(src.x[i]);
        dest.obstacles_y().push_back(src.y[i]);
    }
    dest.obstacles_number(MAX_OBSTACLES);
}

int main() {
    const char *log_folder = "log"; // Path to the log folder
    const char *log_file = "log/obstacle_log.txt"; // Path to the log file

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    // Initialize DDS publisher
    ObstaclesPublisher* publisher = new ObstaclesPublisher();
    if (!publisher->init("ObstaclesTopic")) {
        error_exit("Failed to initialize DDS Publisher");
    }

    Obstacle obstacles; // Structure to hold obstacle data
    Obstacles dds_obstacles; // DDS message type
    init_obstacles(&obstacles); // Initialize the obstacle structure
    publisher->waitSub(); // Wait for the subscriber to connect
    time_t last_generation_time = 0; // Last time obstacles were generated
    time_t last_log_time = 0; // Last time execution was logged
    while (1) {
        time_t current_time = time(NULL); // Get the current time

        // Generate new obstacles every 10 seconds
        if (current_time - last_generation_time >= 10) {
            generate_obstacles(&obstacles);
            convert_to_dds_obstacles(obstacles, dds_obstacles); // Convert to DDS message type

            last_generation_time = current_time; // Update the last generation time
        }
            publisher->publish(dds_obstacles); // Publish obstacle data using DDS

        // Log execution every second
        if (current_time - last_log_time >= 1) {
            log_execution(log_file);
            last_log_time = current_time; // Update the last log time
        }
    }
    delete publisher;
    return 0;
}