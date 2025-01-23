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

// Initialize obstacles and targets
void init_obstacles(Obstacle obstacles[]);
void init_targets(Target targets[]);

// Randomly generate obstacles and targets
void generate_obstacles(Obstacle obstacles[]);
void generate_targets(Target targets[]);

// Draw obstacles and targets
void draw_obstacles(Obstacle obstacles[]);
void draw_targets(Target targets[]);

#endif
