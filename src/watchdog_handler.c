#include "watchdog_handler.h"

const char *log_files[LOG_FILES_COUNT] = {
    "log/target_log.txt",
    "log/output_window_log.txt",
    "log/input_window_log.txt",
    "log/server_log.txt",
    "log/obstacle_log.txt"
};

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void check_deadlines(int fd_signal) {
    char signal = 'k';
    time_t current_time = time(NULL);
    if (current_time == (time_t)-1) {
        error_exit("Failed to get current time");
    }

    for (int i = 0; i < LOG_FILES_COUNT; i++) {
        FILE *log_fp = fopen(log_files[i], "r");
        if (log_fp == NULL) {
            error_exit("Failed to open log file");
        }
        char line[256];
        time_t last_time = 0;
        while (fgets(line, sizeof(line), log_fp)) {
            char *time_str = strstr(line, "Time: ");
            if (time_str != NULL) {
                time_str += 6; // Move past "Time: "
                last_time = (time_t)atol(time_str);
            }
        }
        fclose(log_fp);
        if (current_time - last_time > TIMEOUT) {
            write(fd_signal, &signal , sizeof(signal)); // Write to signal buffer
        }
    }
}