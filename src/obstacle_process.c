#include "obstacle_handler.h"

int main() {
    const char *obstacle_receive = "/tmp/obstacle_receive";
    const char *log_folder = "log";
    const char *log_file = "log/obstacle_log.txt";

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    int fd_receive = open(obstacle_receive, O_WRONLY);
    if (fd_receive < 0) {
        error_exit("Failed to open named pipe");
    }

    Obstacle obstacles;
    init_obstacles(&obstacles);

    time_t last_generation_time = 0;
    time_t last_log_time = 0;
    while (1) {
        time_t current_time = time(NULL);

        if (current_time - last_generation_time >= 10) {
            generate_obstacles(&obstacles);
            // Send the entire Obstacle struct
            if (write(fd_receive, &obstacles, sizeof(obstacles)) < 0) {
                error_exit("Failed to write to named pipe");
            }
            last_generation_time = current_time;
        }

        if (current_time - last_log_time >= 1) {
            log_execution(log_file);
            last_log_time = current_time;
        }
    }

    close(fd_receive);
    return 0;
}