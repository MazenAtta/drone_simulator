#include "watchdog_handler.hpp"

// Array of log file paths
const char *log_files[LOG_FILES_COUNT] = {
    "log/output_window_log.txt",
    "log/input_window_log.txt",
    "log/server_log.txt",
};

// Function to print an error message and exit the program
void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Function to check deadlines for log files and send a signal if a deadline is missed
void check_deadlines(int fd_signal) {
    char signal = 'k'; // Signal to be sent if a deadline is missed
    time_t current_time = time(NULL); // Get the current time
    if (current_time == (time_t)-1) {
        error_exit("Failed to get current time"); // Handle error if current time cannot be retrieved
    }

    // Loop through each log file
    for (int i = 0; i < LOG_FILES_COUNT; i++) {
        FILE *log_fp = fopen(log_files[i], "r"); // Open the log file for reading
        if (log_fp == NULL) {
            error_exit("Failed to open log file"); // Handle error if log file cannot be opened
        }
        char line[256]; // Buffer to store each line read from the log file
        time_t last_time = 0; // Variable to store the last recorded time from the log file
        // Read each line from the log file
        while (fgets(line, sizeof(line), log_fp)) {
            char *time_str = strstr(line, "Time: "); // Find the "Time: " substring in the line
            if (time_str != NULL) {
                time_str += 6; // Move past "Time: " to get the actual time string
                last_time = (time_t)atol(time_str); // Convert the time string to a time_t value
            }
        }
        fclose(log_fp); // Close the log file
        // Check if the time difference exceeds the timeout
        if (current_time - last_time > TIMEOUT) {
            //printf("Deadline missed for %s\n", log_files[i]); // Print a message to the console
            //write(fd_signal, &signal , sizeof(signal)); // Write the signal to the signal buffer
        }
    }
}