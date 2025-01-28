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

    while (1) {
        generate_obstacles(&obstacles);
        // Send the entire Obstacle struct
        if (write(fd_receive, &obstacles, sizeof(obstacles)) < 0) {
            error_exit("Failed to write to named pipe");
        }

        log_execution(log_file);

        sleep(10); // Sleep for 10 seconds before generating new obstacles
    }

    close(fd_receive);
    return 0;
}