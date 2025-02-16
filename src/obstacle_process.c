#include "obstacle_handler.h"

int main() {
    const char *obstacle_receive = "/tmp/obstacle_receive"; // Path to the named pipe for obstacle data
    const char *log_folder = "log"; // Path to the log folder
    const char *log_file = "log/obstacle_log.txt"; // Path to the log file

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    // Open the named pipe for writing
    int fd_receive = open(obstacle_receive, O_WRONLY | O_NONBLOCK);
    if (fd_receive < 0) {
        error_exit("Failed to open named pipe");
    }

    Obstacle obstacles; // Structure to hold obstacle data
    init_obstacles(&obstacles); // Initialize the obstacle structure

    time_t last_generation_time = 0; // Last time obstacles were generated
    time_t last_log_time = 0; // Last time execution was logged
    while (1) {
        time_t current_time = time(NULL); // Get the current time

        // Generate new obstacles every 10 seconds
        if (current_time - last_generation_time >= 10) {
            generate_obstacles(&obstacles);
            // Send the entire Obstacle struct through the named pipe
            if (write(fd_receive, &obstacles, sizeof(obstacles)) < 0) {
                error_exit("Failed to write to named pipe");
            }
            last_generation_time = current_time; // Update the last generation time
        }

        // Log execution every second
        if (current_time - last_log_time >= 1) {
            log_execution(log_file);
            last_log_time = current_time; // Update the last log time
        }
    }

    close(fd_receive); // Close the named pipe
    return 0;
}