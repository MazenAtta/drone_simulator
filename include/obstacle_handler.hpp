#ifndef OBSTACLE_HANDLER_H
#define OBSTACLE_HANDLER_H

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

void init_obstacles(Obstacle *obstacles);
void generate_obstacles(Obstacle *obstacles);
void error_exit(const char *msg);
void log_execution(const char *log_file);

#endif // OBSTACLE_HANDLER_H