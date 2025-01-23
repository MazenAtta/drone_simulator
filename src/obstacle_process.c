#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define COLS 80
#define LINES 25
#define MAX_OBSTACLES 10

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
        obstacles->x[i] = rand() % COLS;
        obstacles->y[i] = rand() % LINES;
    }
}

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    const char *obstacle_receive = "/tmp/obstacle_receive";

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
        sleep(10); // Sleep for 10 seconds before generating new obstacles
    }

    close(fd_receive);
    return 0;
}