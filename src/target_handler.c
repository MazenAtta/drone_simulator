#include "target_handler.h"

void init_targets(Target *targets) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        targets->x[i] = -1; // Off-screen by default
        targets->y[i] = -1;
        targets->id[i] = i + 1;
    }
}

void generate_targets(Target *targets) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        targets->x[i] = BORDER_MARGIN + rand() % (COLS - 2 * BORDER_MARGIN);
        targets->y[i] = BORDER_MARGIN + rand() % (LINES - 2 * BORDER_MARGIN);
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