#include "target_handler.h"

int main() {
    const char *target_receive = "/tmp/target_receive";
    const char *log_folder = "log";
    const char *log_file = "log/target_log.txt";

    // Create log folder if it doesn't exist
    mkdir(log_folder, 0777);

    int fd_receive = open(target_receive, O_WRONLY);
    if (fd_receive < 0) {
        error_exit("Failed to open named pipe");
    }

    Target targets;
    init_targets(&targets);

    while (1) {
        generate_targets(&targets);
        // Send the entire Target struct
        if (write(fd_receive, &targets, sizeof(targets)) < 0) {
            error_exit("Failed to write to named pipe");
        }

        log_execution(log_file);

        sleep(10); // Sleep for 10 seconds before generating new targets
    }

    close(fd_receive);
    return 0;
}