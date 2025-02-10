#include "obstacle_handler.hpp"

// Initialize obstacles to be off-screen by default
void init_obstacles(Obstacle *obstacles) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles->x[i] = -1; // Off-screen by default
        obstacles->y[i] = -1;
    }
}

// Generate random positions for obstacles within the screen boundaries
void generate_obstacles(Obstacle *obstacles) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles->x[i] = BORDER_MARGIN + rand() % (COLS - 2 * BORDER_MARGIN);
        obstacles->y[i] = BORDER_MARGIN + rand() % (LINES - 2 * BORDER_MARGIN);
    }
}

// Print an error message and exit the program
void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Log the execution details to a specified log file
void log_execution(const char *log_file) {
    FILE *log_fp = fopen(log_file, "a");
    if (log_fp == NULL) {
        error_exit("Failed to open log file");
    }

    time_t now = time(NULL);
    if (now == (time_t)-1) {
        error_exit("Failed to get current time");
    }

    // Log the process ID and current time
    fprintf(log_fp, "PID: %d, Time: %ld\n", getpid(), now);
    fclose(log_fp);
}