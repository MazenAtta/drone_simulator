#include "obstacle_handler.h"

void init_obstacles(Obstacle *obstacles) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles->x[i] = -1; // Off-screen by default
        obstacles->y[i] = -1;
    }
}

void generate_obstacles(Obstacle *obstacles) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles->x[i] = BORDER_MARGIN + rand() % (COLS - 2 * BORDER_MARGIN);
        obstacles->y[i] = BORDER_MARGIN + rand() % (LINES - 2 * BORDER_MARGIN);
    }
}

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void log_execution(const char *log_file) {
    FILE *log_fp = fopen(log_file, "a");
    if (log_fp == NULL) {
        error_exit("Failed to open log file");
    }

    time_t now = time(NULL);
    if (now == (time_t)-1) {
        error_exit("Failed to get current time");
    }

    fprintf(log_fp, "PID: %d, Time: %ld\n", getpid(), now);
    fclose(log_fp);
}