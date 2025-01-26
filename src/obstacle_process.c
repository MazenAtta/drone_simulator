#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define COLS 80
#define LINES 25
#define MAX_OBSTACLES 10
#define BORDER_MARGIN 2

typedef struct {
    int x[MAX_OBSTACLES], y[MAX_OBSTACLES];
} Obstacle;

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

int main() {
    const char *obstacle_receive = "/tmp/obstacle_receive";
    const char *log_folder = "log";
    const char *log_file = "log/obstacles_log.txt";

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