#ifndef OBSTACLE_TARGET_HANDLER_H
#define OBSTACLE_TARGET_HANDLER_H

#include <ncurses.h>

#define MAX_OBSTACLES 10
#define MAX_TARGETS 5

typedef struct {
    int x[MAX_OBSTACLES], y[MAX_OBSTACLES];
} Obstacle;

typedef struct {
    int x[MAX_TARGETS], y[MAX_TARGETS];
    int id[MAX_TARGETS];
} Target;



#endif
