#ifndef OUTPUT_WINDOW_HANDLER_H
#define OUTPUT_WINDOW_HANDLER_H

#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#define DRONE_MASS 1.0
#define DRONE_FRICTION 1.0
#define COMMAND_FORCE_STEP 1.0
#define REPULSIVE_CONSTANT 10.0
#define ATTRACTIVE_CONSTANT 0.1
#define PERCEPTION_RADIUS 2.0
#define TIME_STEP .1  // 1 s as integration interval

#define DRONE_SYMBOL '+'
#define FORCE_UP -1
#define FORCE_DOWN 1
#define FORCE_LEFT -1
#define FORCE_RIGHT 1

#define MAX_OBSTACLES 10
#define MAX_TARGETS 5

typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    float prev_x, prev_y;
    float command_force_x, command_force_y;
    float prev_total_command_force_x, prev_total_command_force_y;
    float force_x, force_y;
    float score;
} Drone;

typedef struct {
    int command;
    int Obstacle_x[MAX_OBSTACLES], Obstacle_y[MAX_OBSTACLES];
    int Target_x[MAX_TARGETS], Target_y[MAX_TARGETS], target_id[MAX_TARGETS];
    int game_pause;
    int game_start;
    int game_over;
    int game_reset;
    int score;
    int level;
} Game;

typedef struct {
    float drone_prev_x, drone_prev_y;
    float obstacle_prev_x[MAX_OBSTACLES], obstacle_prev_y[MAX_OBSTACLES];
    float target_prev_x[MAX_TARGETS], target_prev_y[MAX_TARGETS];
} GamePrev;

typedef struct {
    int x[MAX_OBSTACLES], y[MAX_OBSTACLES];
} Obstacle;

typedef struct {
    int x[MAX_TARGETS], y[MAX_TARGETS];
    int id[MAX_TARGETS];
} Target;

void init_ncurses();
void log_execution(const char *log_file);
void draw_drone(Drone *drone);
void draw_obstacles(Obstacle *obstacles);
void draw_targets(Target *targets);
void draw_border();
void init_drone(Drone *drone);
void update_drone_state(Drone *drone, char command);
void calculate_total_forces(Drone *drone, Obstacle *obstacles, Target *targets);
void update_drone(Drone *drone);
void clear_screen(Drone *drone, Obstacle *obstacles, Target *targets, GamePrev *game_prev);
void score(Drone *drone, Target *targets);

#endif // OUTPUT_WINDOW_HANDLER_H