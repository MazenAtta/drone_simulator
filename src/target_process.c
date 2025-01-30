#include "target_handler.h"

int main() {
    const char *target_receive = "/tmp/target_receive"; // Path to the named pipe for receiving targets
    const char *log_folder = "log"; // Path to the log folder
    const char *log_file = "log/target_log.txt"; // Path to the log file

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    // Open the named pipe for writing
    int fd_receive = open(target_receive, O_WRONLY | O_NONBLOCK);
    if (fd_receive < 0) {
        error_exit("Failed to open named pipe");
    }

    Target targets; // Structure to hold target data
    init_targets(&targets); // Initialize the targets

    time_t last_generation_time = 0; // Last time targets were generated
    time_t last_log_time = 0; // Last time execution was logged
    while (1) {
        time_t current_time = time(NULL); // Get the current time

        // Generate new targets every 10 seconds
        if (current_time - last_generation_time >= 10) {
            generate_targets(&targets);
            // Send the entire Target struct through the named pipe
            if (write(fd_receive, &targets, sizeof(targets)) < 0) {
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