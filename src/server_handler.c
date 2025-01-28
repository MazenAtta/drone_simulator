#include "server_handler.h"

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

void update_game_state(Game *game, Obstacle *obstacles, Target *targets) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        game->Obstacle_x[i] = obstacles->x[i];
        game->Obstacle_y[i] = obstacles->y[i];
    }
    for (int i = 0; i < MAX_TARGETS; i++) {
        game->Target_x[i] = targets->x[i];
        game->Target_y[i] = targets->y[i];
        game->target_id[i] = targets->id[i];
    }
}

int open_pipe(const char *pipe_name, int flags) {
    int fd = open(pipe_name, flags);
    if (fd < 0) {
        perror("Failed to open named pipe");
        exit(EXIT_FAILURE);
    }
    return fd;
}